#include <Core/Memory/GarbageCollector.h>
#include <Core/Common.h>

static TOS_Thread* _thread  = NULL;
static uint32_t    _tm_last = 0;
static uint32_t    _timer   = 0;

int TOS_RunGarbageCollector(TOS_PtrList* args)
{
    TOS_Log("%s Entered GC main\n", DEBUG_INFO);

    while (true)
    {
        uint32_t tm = TOS_GetMillisecondsNow(NULL);
        if (_tm_last != tm) { _timer++; _tm_last = tm; }
        
        if (_timer >= 32)
        {
            _timer = 0;
            TOS_LockThread(THIS_THREAD);
            TOS_GarbageCollect(true);
            TOS_SwitchThread(true);
        }

        TOS_SwitchThread(true);
    }

    return 0;
}

void TOS_StartGarbageCollector()
{
    _thread = TOS_NewThread("memgc", 64 * KILOBYTE, TOS_RunGarbageCollector, THREAD_PRIORITY_NORMAL, TOS_NewPtrList());
    TOS_LoadThread(_thread);
    TOS_StartThread(_thread);
}

void TOS_GarbageCollect(bool notify)
{    
    size_t blks_collected = 0;

    TOS_Heap* heaps = TOS_GetHeaps();
    for (size_t i = 0; i < TOS_GetMemBlockCount(); i++) { blks_collected += TOS_HeapCollect(&heaps[i]); }

    if (notify && (blks_collected > 0)) 
    { 
        TOS_Log("%s Collected %u unused heap blocks\n", DEBUG_INFO, blks_collected); 
    }
}
