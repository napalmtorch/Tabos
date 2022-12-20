#include <Core/Multitasking/Thread.h>
#include <Core/Common.h>

#define STACKSZ_MIN (4 * KILOBYTE)
#define STACKSZ_MAX (8 * MEGABYTE)

TOS_Thread* THIS_THREAD = NULL;
TOS_Thread* NEXT_THREAD = NULL;

static int _tmlast;

void TOS_OnThreadReturn()
{
    register int eax asm("eax");   
    int exitcode = eax;

    asm volatile("cli");
    TOS_TerminateThread(THIS_THREAD);
    TOS_Log("%s Thread %x(%s) has exited with code %d\n", DEBUG_INFO, THIS_THREAD->id, THIS_THREAD->name, exitcode);

    asm volatile("sti");
    while (true) { TOS_SwitchThread(true); } 
}

TOS_Thread* TOS_NewThread(const char* name, size_t stacksz, TOS_ThreadProtocol protocol, THREAD_PRIORITY priority, TOS_PtrList args)
{    
    TOS_Thread* thread = TOS_Alloc(sizeof(TOS_Thread*));
    thread->name        = TOS_NewString(name);
    thread->stacksz     = TOS_Clamp(stacksz, STACKSZ_MIN, STACKSZ_MAX);
    thread->stack       = TOS_Alloc(thread->stacksz);
    thread->id          = TOS_GenerateThreadID();
    thread->protocol    = protocol;
    thread->priority    = priority;
    thread->lock        = UNLOCKED;
    thread->state       = THREAD_HALTED;
    thread->args        = TOS_DuplicatePtrList(&args);
    thread->time        = (TOS_ThreadTime) { 0, 0, 0 };
    
    uint32_t* stk = (uint32_t*)((uintptr_t)thread->stack + (thread->stacksz - sizeof(TOS_ThreadRegisters) - 16));
    *--stk = (uint32_t)thread;
    *--stk = (uint32_t)&thread->args;
    *--stk = (uint32_t)TOS_OnThreadReturn;
    *--stk = (uint32_t)protocol;

    thread->registers.edx    = (uint32_t)thread;
    thread->registers.esp    = (uint32_t)stk;
    thread->registers.eflags = (uint32_t)0x202;

    TOS_Log("%s Ptr:%p Stack:%p-%p Entry:%p Args:%u ID:%x Name:%s\n", DEBUG_THREAD, thread, thread->stack, (uint32_t)thread->stack + thread->stacksz, thread->protocol, thread->args.count, thread->id, thread->name);
    return thread;
}

void TOS_FreeThread(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_FreeThread(00000000) - Attempt to free null thread"); return; }
    TOS_FreePtrList(&thread->args, false);
    TOS_Free(thread->name);
    TOS_Free(thread->stack);
    TOS_Free(thread);
}

void TOS_UpdateThreadTime(TOS_Thread* thread)
{
    if (thread == NULL) { TOS_Panic("TOS_UpdateThreadTime(00000000) - Attempt to update null thread time"); return; }
    thread->time.ticks++;

    TOS_DateTime tm = TOS_GetTimeNow(NULL);
    if (_tmlast != tm.second)
    {
        _tmlast            = tm.second;
        thread->time.tps   = thread->time.ticks;
        thread->time.ticks = 0;
    }
}