#pragma once
#include <Core/Memory/Heap.h>

#define HEAPS_MAX    32
#define HEAP_MAXENTS 24576

void TOS_InitHeapManager();
void TOS_RegisterHeap(TOS_Heap* heap);

size_t TOS_GetHeapCount();
TOS_Heap* TOS_GetHeaps();