#pragma once
#include <Lib/Types.h>

#define MEMBLK_MAX 1024

typedef enum
{
    MEM_INVALID,
    MEM_FREE,
    MEM_RESERVED,
    MEM_ACPI_RECLAIM,
    MEM_NVS,
    MEM_BADRAM,
    MEM_VRAM,
    MEM_KERNEL,
    MEM_MODULE,
    MEM_PFA,
    MEM_HEAP,
    MEM_ALLOC,
    MEM_UNUSED,
    MEM_COUNT,
} MEMORY_TYPE;

typedef struct
{
    uintptr_t   addr;
    size_t      sz;
    MEMORY_TYPE type;
} TOS_MemoryBlock;

void TOS_InitMemMgr(bool zero_free);
void TOS_ClearFreeMemory();

TOS_MemoryBlock* TOS_RequestMemBlock(size_t sz);
void TOS_ReleaseMemBlock(TOS_MemoryBlock* blk);

TOS_MemoryBlock* TOS_MapMemBlock(TOS_MemoryBlock blk);
void TOS_UnmapMemBlock(TOS_MemoryBlock* blk);

TOS_MemoryBlock* TOS_GetMemBlock(uintptr_t addr);
TOS_MemoryBlock* TOS_GetMemBlockAt(int index);

TOS_MemoryBlock* TOS_GetMemBlockByType(MEMORY_TYPE type);

TOS_MemoryBlock* TOS_GetMemBlocks();

size_t TOS_GetMemBlockCount();
const char* TOS_GetMemTypeName(MEMORY_TYPE type);
