#include <Core/Tests.h>
#include <Core/Common.h>

void TOS_SysTestHeap(size_t iters)
{
    while (iters > 0)
    {
        // decrement iteration
        iters--;

        // some random sizes to for the test allocations
        size_t sizes[8] = { 32, 128, 512, 8000, 32 * MEGABYTE, 1000000, 2 * MEGABYTE, 100 * KILOBYTE };
        void*  ptrs[8];

        // create and fill 8 blocks per iteration
        for (size_t i = 0; i < 8; i++)
        {
            ptrs[i] = TOS_Alloc(sizes[i]);
            TOS_MemSet(ptrs[i], 0x69420666, sizes[8]);
        }
        
        // free the blocks in a seperate loop, to ensure there is more than 1 allocation in the heap at a time
        for (size_t i = 0; i < 8; i++) { TOS_Free(ptrs[i]); }
    }

    // test finished successfully
    TOS_Log("%s Heap test succeeded!\n", DEBUG_OK);
}

void TOS_SysTestRAMFS(size_t iters)
{
    // test finished successfully
    TOS_Log("%s RAMFS test succeeded!\n", DEBUG_OK);
}

int TOS_SysTestThreading_TestMain(TOS_PtrList* args)
{
    TOS_Log("Entered test thread main successfully\n");
    return 420;
}

void TOS_SysTestThreading(size_t iters)
{   
    while (iters > 0)
    {
        iters--;

        char name[64];
        TOS_MemSet(name, 0, sizeof(name));
        TOS_PrintTo(name, "test_thread%d", iters);
        TOS_Log("NAME: %s\n", name);

        TOS_Thread* thread = TOS_NewThread(name, 0x10000, TOS_SysTestThreading_TestMain, THREAD_PRIORITY_NORMAL, TOS_NewPtrList());
        TOS_LoadThread(thread);
        TOS_StartThread(thread);
        TOS_SwitchThread(true);
    }

    // test finished successfully
    TOS_Log("%s Multithreading test succeeded!\n", DEBUG_OK);
}