#include "Core/Multitasking/Scheduler.h"
#include <Core/Common.h>

extern void main(void*);
extern void _context_switch();

static TOS_PtrList _threads;
static int        _active;
static uint32_t   _tid;
static bool       _modifying;
static bool       _ready;

void TOS_InitScheduler()
{
    _threads   = TOS_NewPtrList();
    _tid       = 0;
    _active    = 0;
    _modifying = false;
    _ready     = false;
    TOS_Log("%s Initialized scheduler\n", DEBUG_OK);
}

void TOS_InitKernelThread()
{
    TOS_Thread* thread = TOS_Alloc(sizeof(TOS_Thread));
    thread->name      = TOS_NewString("kernel");
    thread->id        = _tid++;
    thread->priority  = THREAD_PRIORITY_NORMAL;
    thread->state     = THREAD_RUNNING;
    thread->protocol  = (TOS_ThreadProtocol)main;
    thread->stack     = (uint32_t*)TOS_GetKernelStackBottom();
    thread->stacksz   = TOS_GetKernelStackSize();
    TOS_Log("%s Ptr:%p Stack:%p-%p Entry:%p Args:%u ID:%x Name:%s\n", DEBUG_THREAD, thread, thread->stack, (uint32_t)thread->stack + thread->stacksz, thread->protocol, thread->args.count, thread->id, thread->name);
    TOS_LoadThread(thread);
}

void TOS_SwitchThread(bool unlock)
{
    if (_modifying || !_ready) { return; }
    asm volatile("cli");
    _modifying = true;

    THIS_THREAD = TOS_PtrListAt(&_threads, _active);
    if (THIS_THREAD->lock == LOCKED)
    {
        if (!unlock) { asm volatile("sti"); _modifying = false; return; }
        THIS_THREAD->lock = UNLOCKED;
    }

    while (true)
    {
        _active++;
        TOS_Thread* thread = TOS_PtrListAt(&_threads, _active);

        if ((size_t)_active >= _threads.count) { _active = 0; NEXT_THREAD = TOS_PtrListAt(&_threads, 0); break; }
        if (thread == NULL) { TOS_Panic("Null thread located in scheduler"); return; }
        if (thread->state == THREAD_HALTED || thread->state == THREAD_WAITING) { continue; }
        if (thread->state == THREAD_TERMINATED) { TOS_UnloadThread(thread); continue; }
        NEXT_THREAD = thread;
        break;
    }

    _modifying = false;
    _context_switch();
}

void TOS_LoadThread(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_LoadThread(00000000) - Attempt to load null thread into scheduler"); return; }
    _modifying = true;
    TOS_PtrListAdd(&_threads, thread);
    TOS_Log("%s Loaded thread %x(%s) at %p\n", DEBUG_INFO, thread->id, thread->name, thread);
    _modifying = false;
}

void TOS_UnloadThread(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_UnloadThread(00000000) - Attempt to unload null thread from scheduler"); return; }
    _modifying = true;
    for (size_t i = 0; i < _threads.count; i++)
    {
        TOS_Thread* t = TOS_PtrListAt(&_threads, i);
        if (thread == t)
        {
            TOS_Log("%s Unloading thread %x(%s) at %p\n", DEBUG_INFO, thread->id, thread->name, thread);
            TOS_PtrListErase(&_threads, i, false);
            TOS_FreeThread(thread);
            _modifying = false;
            return;
        }
    }
    _modifying = false;
    TOS_Panic("TOS_UnloadThread(%p) - Failed to unload thread", thread);
}

void TOS_StartThread(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_StartThread(00000000) - Attempt to start null thread"); return; }
    if (thread->state == THREAD_RUNNING) { return; }
    _modifying = true;
    thread->state = THREAD_RUNNING;
    TOS_Log("%s Started thread %x(%s) at %p\n", DEBUG_INFO, thread->id, thread->name, thread);
    _modifying = false;
}

void TOS_StopThread(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_StopThread(00000000) - Attempt to stop null thread"); return; }
    if (thread->state == THREAD_HALTED) { return; }
    _modifying = true;
    thread->state = THREAD_HALTED;
    TOS_Log("%s Stopped thread %x(%s) at %p\n", DEBUG_INFO, thread->id, thread->name, thread);
    _modifying = false;
}

void TOS_TerminateThread(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_StopThread(00000000) - Attempt to stop null thread"); return; }
    if (thread->state == THREAD_TERMINATED) { return; }
    _modifying = true;
    thread->state = THREAD_TERMINATED;
    TOS_Log("%s Terminated thread %x(%s) at %p\n", DEBUG_INFO, thread->id, thread->name, thread);
    _modifying = false;
}

void TOS_ToggleScheduler(bool enabled) { _ready = enabled; }

uint32_t TOS_GenerateThreadID() { return _tid++; }

uint32_t TOS_GetThreadCount() { return _threads.count; }