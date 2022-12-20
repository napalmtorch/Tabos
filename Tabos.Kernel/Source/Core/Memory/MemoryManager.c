#include <Core/Memory/MemoryManager.h>
#include <Core/Common.h>

static const char* MEMTYPE_NAMES[] = 
{
    "invalid     ",
    "free        ",
    "reserved    ",
    "acpi_reclaim",
    "nvs         ",
    "bad_ram     ",
    "video_ram   ",
    "kernel      ",
    "module      ",
    "pgfrm_alloc ",
    "heap        ",
    "allocation  ",
    "unused      ",
};

static TOS_MemoryBlock _blocks[MEMBLK_MAX];
static size_t         _count;

void TOS_InitMemMgr(bool zero_free)
{
    TOS_MemSet(_blocks, 0, sizeof(_blocks));
    _count = 0;

    TOS_MemoryMapEntry*  mmap   = TOS_GetMultiboot()->mmap;
    size_t               mmapsz = TOS_GetMultiboot()->mmap_len / sizeof(TOS_MemoryMapEntry);
    for (size_t i = 0; i < mmapsz; i++)
    {
        if (mmap[i].address + mmap[i].size > UINT32_MAX || mmap[i].size > UINT32_MAX || mmap[i].size == 0) { continue; }
        if (mmap[i].address == TOS_GetKernelStart())
        {
            TOS_MapMemBlock((TOS_MemoryBlock){ (uint32_t)mmap[i].address, TOS_GetKernelSize(), MEM_KERNEL });
            TOS_MapMemBlock((TOS_MemoryBlock){ (uint32_t)mmap[i].address + TOS_GetKernelSize(), (uint32_t)mmap[i].size - TOS_GetKernelSize(), MEM_FREE });
        }
        else { TOS_MapMemBlock((TOS_MemoryBlock){ (uint32_t)mmap[i].address, (uint32_t)mmap[i].size, ((mmap[i].address < TOS_GetKernelStart() && mmap[i].type == MEM_FREE) ? MEM_UNUSED : mmap[i].type) }); }
    }

    TOS_MultibootModule* mods = TOS_GetMultiboot()->modules;
    for (size_t i = 0; i < TOS_GetMultiboot()->modules_count; i++)
    {
        if (mods[i].address_start != 0 && mods[i].address_end != 0)
        {
            TOS_MapMemBlock((TOS_MemoryBlock){ mods[i].address_start, mods[i].address_end - mods[i].address_start, MEM_MODULE });
        }
    }

    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].addr == mods[0].address_start && _blocks[i].type == MEM_FREE)
        {
            size_t sz = mods[0].address_end - mods[0].address_start;
            _blocks[i].addr += sz;
            _blocks[i].sz   -= sz;
        }
    }

    if (zero_free) { TOS_ClearFreeMemory(); }
    TOS_Log("%s Kernel memory - Region:%p:%p, Size:%uKB\n", DEBUG_INFO, TOS_GetKernelStart(), TOS_GetKernelEnd(), TOS_GetKernelSize() / KILOBYTE);
    TOS_Log("%s Kernel stack  - Region:%p:%p, Size:%uKB\n", DEBUG_INFO, TOS_GetKernelStackBottom(), TOS_GetKernelStackTop(), TOS_GetKernelStackSize() / KILOBYTE);
    TOS_Log("%s Initialized memory manager\n", DEBUG_OK);
}

void TOS_ClearFreeMemory()
{
    TOS_Log("%s Zeroing all available free memory...\n", DEBUG_INFO);

    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].type == MEM_FREE) { TOS_MemSet((void*)_blocks[i].addr, 0, _blocks[i].sz); TOS_Log("Zero-filled %p-%p\n", _blocks[i].addr, _blocks[i].addr + _blocks[i].sz); }
    }
    TOS_Log("%s Finished zeroing available free memory\n", DEBUG_OK);;
}

TOS_MemoryBlock* TOS_RequestMemBlock(size_t sz)
{
    if (sz == 0) { TOS_Panic("TOS_RequestMemBlock(0) - Invalid size"); return NULL; }
    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].type == MEM_FREE && _blocks[i].sz >= sz)
        {
            TOS_MemoryBlock* blk = TOS_MapMemBlock((TOS_MemoryBlock){ _blocks[i].addr, sz, MEM_RESERVED });
            _blocks[i].addr += sz;
            _blocks[i].sz   -= sz;
            TOS_Log("%s Granted request for %a memory block\n", DEBUG_OK, sz);
            return blk;
        }
    }
    TOS_Panic("TOS_RequestMemBlock(%u) - Request for memory block failed", sz);
    return NULL;
}

void TOS_ReleaseMemBlock(TOS_MemoryBlock* blk)
{
    if (blk == NULL) { TOS_Panic("TOS_ReleaseMemBlock(00000000) - Attempt to release null memory block"); return; }
    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].type == MEM_INVALID) { continue; }
        if (&_blocks[i] == blk)
        {
            _blocks[i].type = MEM_FREE;
            TOS_Log("%s Released %a memory block at %p\n", DEBUG_INFO, _blocks[i].sz, _blocks[i].addr);
            return;
        }
    }
    TOS_Panic("TOS_ReleaseMemBlock(%p) - Failed to release memory block", blk);
}

TOS_MemoryBlock* TOS_MapMemBlock(TOS_MemoryBlock blk)
{
    if (blk.sz == 0 || blk.type == MEM_INVALID) { TOS_Panic("TOS_MapMemBlock({ %p, %u, %s }) - Invalid memory block", blk.addr, blk.sz, TOS_GetMemTypeName(blk.type)); return NULL; }
    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].type == MEM_INVALID)
        {
            _blocks[i] = blk;
            _count++;
            TOS_Log("%s I:%u Region:%p-%p Type:%s Size:%a\n", DEBUG_MMAP, i, blk.addr, blk.addr + blk.sz, TOS_GetMemTypeName(blk.type), blk.sz);
            return &_blocks[i];
        }
    }
    TOS_Panic("TOS_MapMemBlock({ %p, %u, %s }) - Failed to map memory block", blk.addr, blk.sz, TOS_GetMemTypeName(blk.type)); 
    return NULL; 
}

void TOS_UnmapMemBlock(TOS_MemoryBlock* blk)
{
    if (blk == NULL) { TOS_Panic("TOS_UnmapMemBlock(00000000) - Attempt to unmap null memory block"); return; }
    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (&_blocks[i] == blk)
        {
            MEMORY_TYPE type = _blocks[i].type;
            _blocks[i] = (TOS_MemoryBlock){ 0, 0, MEM_INVALID };
            _count--;
            TOS_Log("%s I:%u Region:%p-%p Type:%s Size:%a\n", DEBUG_MUNMAP, i, blk->addr, blk->addr + blk->sz, TOS_GetMemTypeName(type), blk->sz);
            return;
        }
    }
    TOS_Panic("TOS_UnmapMemBlock(%p) - Failed to unmap memory block", blk);
}

TOS_MemoryBlock* TOS_GetMemBlock(uintptr_t addr)
{
    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].type == MEM_INVALID) { continue; }
        if (_blocks[i].addr == addr) { return &_blocks[i]; }
    }
    return NULL;
}

TOS_MemoryBlock* TOS_GetMemBlockAt(int index)
{
    if (index < 0 || index >= MEMBLK_MAX) { TOS_Panic("TOS_GetMemBlockAt(%d) - Invalid index", index); return NULL; }
    return &_blocks[index];
}

TOS_MemoryBlock* TOS_GetMemBlockByType(MEMORY_TYPE type)
{
    for (size_t i = 0; i < MEMBLK_MAX; i++) { if (_blocks[i].type == type) { return &_blocks[i]; } }
    return NULL;
}

TOS_MemoryBlock* TOS_GetMemBlocks() { return _blocks; }

size_t TOS_GetMemBlockCount()
{
    size_t count = 0;
    for (size_t i = 0; i < MEMBLK_MAX; i++)
    {
        if (_blocks[i].type == MEM_INVALID) { continue; }
        count++;
    }
    return count;
}

const char* TOS_GetMemTypeName(MEMORY_TYPE type)
{
    return MEMTYPE_NAMES[type >= MEM_COUNT ? MEM_INVALID : type]; 
}
