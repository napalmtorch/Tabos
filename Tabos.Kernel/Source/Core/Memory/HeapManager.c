#include <Core/Memory/HeapManager.h>
#include <Core/Common.h>

static TOS_Heap _heaps[HEAPS_MAX];
static size_t  _count;

void TOS_InitHeapManager()
{
    TOS_MemSet(_heaps, 0, sizeof(_heaps));
    _count = 0;

    TOS_MemoryBlock* blks = TOS_GetMemBlocks();
    for (size_t i = 0; i < TOS_GetMemBlockCount(); i++)
    {
        size_t table_sz = sizeof(TOS_HeapBlock) * HEAP_MAXENTS;
        if (blks[i].type == MEM_FREE && blks[i].sz >= table_sz * 2)
        {
            TOS_InitHeap(&_heaps[_count], blks[i].sz - table_sz, HEAP_MAXENTS, 0x1000, false);
            _count++;
        }
    }   

    TOS_Log("%s Initialized heap manager - %u heaps created\n", DEBUG_OK, _count);
}

size_t TOS_GetHeapCount() { return _count; }

TOS_Heap* TOS_GetHeaps() { return _heaps; }