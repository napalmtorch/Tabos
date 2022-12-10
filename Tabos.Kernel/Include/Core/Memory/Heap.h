#pragma once
#include <Lib/Types.h>
#include <Core/Memory/MemoryManager.h>

typedef struct
{
    uintptr_t   addr;
    size_t      sz;
    MEMORY_TYPE type;
    void*       thread;
} TOS_HeapBlock;

typedef struct
{
    TOS_MemoryBlock* memblk_table;
    TOS_MemoryBlock* memblk_data;
    TOS_HeapBlock*   blocks;
    size_t          alignment;
    size_t          sz, count, max;
    bool            auto_collect;
} TOS_Heap;

void   TOS_InitHeap(TOS_Heap* heap, size_t sz, size_t max_ents, size_t align, bool auto_collect);
void*  TOS_HeapAllocate(TOS_Heap* heap, size_t sz, bool zero_fill);
bool   TOS_HeapFree(TOS_Heap* heap, void* ptr);
size_t TOS_HeapCollect(TOS_Heap* heap);
bool   TOS_HeapMerge(TOS_Heap* heap, TOS_HeapBlock* blk1, TOS_HeapBlock* blk2);

TOS_HeapBlock* TOS_HeapRequestBlock(TOS_Heap* heap, size_t sz);
TOS_HeapBlock* TOS_HeapNextBlock(TOS_Heap* heap);
TOS_HeapBlock* TOS_HeapNearestBlock(TOS_Heap* heap, TOS_HeapBlock* blk);
TOS_HeapBlock* TOS_HeapCreateBlock(TOS_Heap* heap, TOS_HeapBlock blk);
void          TOS_HeapRemoveBlock(TOS_Heap* heap, TOS_HeapBlock* blk);

TOS_HeapBlock* TOS_HeapFetch(TOS_Heap* heap, void* ptr);

size_t TOS_HeapCountType(TOS_Heap* heap, MEMORY_TYPE type);
size_t TOS_HeapAmountFree(TOS_Heap* heap);
size_t TOS_HeapAmountUsed(TOS_Heap* heap);
size_t TOS_HeapSize(TOS_Heap* heap);
