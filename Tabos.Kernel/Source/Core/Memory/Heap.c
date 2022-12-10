#include <Core/Memory/Heap.h>
#include <Core/Common.h>

void TOS_InitHeap(TOS_Heap* heap, size_t sz, size_t max_ents, size_t align, bool auto_collect)
{
    if (heap == NULL) { TOS_Panic("TOS_InitHeap(00000000, %u, %u, %u, %d) - Attempt to access null heap", sz, max_ents, align, auto_collect); return; }
    heap->memblk_table  = TOS_RequestMemBlock(max_ents * sizeof(TOS_HeapBlock));
    heap->memblk_data   = TOS_RequestMemBlock(sz);
    heap->sz            = sz;
    heap->count         = 0;
    heap->max           = max_ents;
    heap->alignment     = align;
    heap->auto_collect  = auto_collect;
    heap->blocks        = (TOS_HeapBlock*)heap->memblk_table->addr;

    TOS_HeapCreateBlock(heap, (TOS_HeapBlock){ heap->memblk_data->addr, heap->sz, MEM_FREE, NULL });

    TOS_Log("%s Heap - Table:%p-%p Data:%p-%p Size:%a Max:%u Align:%p AC:%d\n", DEBUG_OK, 
            heap->memblk_table->addr, heap->memblk_table->addr + heap->memblk_table->sz, heap->memblk_data->addr, heap->memblk_data->addr + heap->memblk_data->sz, heap->sz, heap->max, heap->alignment, heap->auto_collect);
}

void* TOS_HeapAllocate(TOS_Heap* heap, size_t sz, bool zero_fill)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapAllocate(00000000, %a, %d) - Attempt to access null heap", sz, zero_fill); return NULL; }
    if (sz > 0) { sz = TOS_MemAlign(sz, heap->alignment); }

    TOS_HeapBlock* blk = TOS_HeapRequestBlock(heap, sz);
    if (blk == NULL) { return NULL; }

    if (zero_fill) { TOS_MemSet((void*)blk->addr, 0, blk->sz); }
    TOS_Log("%s Region:%p-%p Size:%a\n", DEBUG_MALLOC, blk->addr, blk->addr + blk->sz, blk->sz);
    return (void*)blk->addr;
}

bool TOS_HeapFree(TOS_Heap* heap, void* ptr)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapFree(00000000, %p) - Attempt to access null heap", ptr); return false; }
    if (ptr  == NULL) { TOS_Panic("TOS_HeapFree(%p, 00000000) - Attempt to free null heap block", heap); return false; }

    for (size_t i = 0; i < heap->max; i++)
    {
        if (heap->blocks[i].type == MEM_INVALID) { continue; }
        if (heap->blocks[i].addr == (uintptr_t)ptr)
        {
            heap->blocks[i].type = MEM_FREE;
            TOS_Log("%s Region:%p-%p Size:%a\n", DEBUG_FREE, heap->blocks[i].addr, heap->blocks[i].addr + heap->blocks[i].sz, heap->blocks[i].sz);
            if (heap->auto_collect) 
            { 
                size_t collected = TOS_HeapCollect(heap); 
                //if (collected > 0) { TOS_Log("%s Collected and merged %u free heap blocks\n", DEBUG_INFO, collected); }
            }
            return true;
        }
    }
    return false;
}

size_t TOS_HeapCollect(TOS_Heap* heap)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapCollect(00000000) - Attempt to access null heap"); return 0; }

    size_t c = 0;
    TOS_HeapBlock* nblk;
    for (size_t i = 0; i < heap->max; i++)
    {
        if (heap->blocks[i].type != MEM_FREE) { continue; }
        nblk = TOS_HeapNearestBlock(heap, &heap->blocks[i]);
        if (nblk != NULL) { c += TOS_HeapMerge(heap, &heap->blocks[i], nblk); }
    }

    nblk = TOS_HeapNearestBlock(heap, &heap->blocks[0]);
    if (nblk != NULL) { c += TOS_HeapMerge(heap, &heap->blocks[0], nblk); }

    return c;
}

bool TOS_HeapMerge(TOS_Heap* heap, TOS_HeapBlock* blk1, TOS_HeapBlock* blk2)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapMerge(00000000, %p, %p) - Attempt to access null heap", blk1, blk2); return false; }
    if (blk1 == NULL || blk2 == NULL) { return false; }
    if (blk1->addr > blk2->addr) { blk1->addr = blk2->addr; }
    blk1->sz += blk2->sz;
    TOS_HeapRemoveBlock(heap, blk2);
    return true;
}

TOS_HeapBlock* TOS_HeapRequestBlock(TOS_Heap* heap, size_t sz)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapSize(00000000, %a) - Attempt to access null heap", sz); return NULL; }
    if (sz   == 0)    { TOS_Panic("TOS_HeapRequestBlock(%p, 0) - Attempt to request 0 byte heap block", heap); return NULL; }
    
    for (size_t i = 0; i < heap->max; i++)
    {
        if (heap->blocks[i].sz >= sz && heap->blocks[i].type == MEM_FREE)
        {
            TOS_HeapBlock* blk     = TOS_HeapCreateBlock(heap, (TOS_HeapBlock){ heap->blocks[i].addr, sz, MEM_ALLOC, NULL });
            heap->blocks[i].addr += sz;
            heap->blocks[i].sz   -= sz;
            return blk;
        }
    }
    return NULL;
}

TOS_HeapBlock* TOS_HeapNextBlock(TOS_Heap* heap)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapNextBlock(00000000) - Attempt to access null heap"); return NULL; }
    
    for (size_t i = 0; i < heap->max; i++) { if (heap->blocks[i].type == MEM_INVALID) { return &heap->blocks[i]; } }

    TOS_Panic("TOS_HeapNextBlock(%p) - Maximum amount of heap blocks reached", heap);
    return NULL;
}

TOS_HeapBlock* TOS_HeapNearestBlock(TOS_Heap* heap, TOS_HeapBlock* blk)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapNearestBlock(00000000, %p) - Attempt to access null heap", blk); return NULL; }
    if (blk  == NULL) { return NULL; }

    for (size_t i = 0; i < heap->max; i++)
    {
        if (heap->blocks[i].type != MEM_FREE) { continue; }
        if (heap->blocks[i].addr + heap->blocks[i].sz == blk->addr) { return &heap->blocks[i]; }
        if (blk->addr - blk->sz == heap->blocks[i].addr) { return &heap->blocks[i]; }
    }
    return NULL;
}

TOS_HeapBlock* TOS_HeapCreateBlock(TOS_Heap* heap, TOS_HeapBlock blk)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapCreateBlock(00000000, %p) - Attempt to access null heap", blk); return NULL; }
    if (blk.sz == 0 || blk.addr == 0) { TOS_Panic("TOS_HeapCreateBlock(%p, 00000000) - Attempt to create invalid heap block", heap); return NULL; }

    TOS_HeapBlock* p_blk = TOS_HeapNextBlock(heap);
    *p_blk = blk;
    heap->count++;
    return p_blk;
}

void TOS_HeapRemoveBlock(TOS_Heap* heap, TOS_HeapBlock* blk)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapRemoveBlock(00000000, %p) - Attempt to access null heap", blk); return; }
    if (blk  == NULL) { TOS_Panic("TOS_HeapRemoveBlock(%p, 00000000) - Attempt to remove null block from heap", heap); return; }

    for (size_t i = 0; i < heap->max; i++)
    {
        if (&heap->blocks[i] == blk)
        {
            heap->blocks[i] = (TOS_HeapBlock){ 0, 0, MEM_INVALID, NULL };
            heap->count--;
            return;
        }
    }
    TOS_Panic("TOS_HeapRemoveBlock(%p, %p) - Failed to remove heap block", heap, blk);
}

TOS_HeapBlock* TOS_HeapFetch(TOS_Heap* heap, void* ptr)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapFetch(00000000, %p) - Attempt to access null heap", ptr); return NULL; }
    if (ptr  == NULL) { return NULL;}

    for (size_t i = 0; i < heap->max; i++)
    {
        if (heap->blocks[i].type != MEM_ALLOC) { continue; }
        if (heap->blocks[i].addr == (uintptr_t)ptr) { return &heap->blocks[i]; }
    }
    return NULL;
}

size_t TOS_HeapCountType(TOS_Heap* heap, MEMORY_TYPE type)
{
    if (heap == NULL) { TOS_Panic("TOS_HeapCountType(00000000) - Attempt to access null heap"); return 0; }

    size_t sz = 0;
    for (size_t i = 0; i < heap->max; i++)
    {
        if (heap->blocks[i].type != type) { continue; }
        sz += heap->blocks[i].sz;
    }
    return sz;
}

size_t TOS_HeapAmountFree(TOS_Heap* heap) { return TOS_HeapCountType(heap, MEM_FREE); }

size_t TOS_HeapAmountUsed(TOS_Heap* heap) { return TOS_HeapCountType(heap, MEM_ALLOC); }

size_t TOS_HeapSize(TOS_Heap* heap) { return TOS_HeapCountType(heap, MEM_FREE) + TOS_HeapCountType(heap, MEM_ALLOC); }