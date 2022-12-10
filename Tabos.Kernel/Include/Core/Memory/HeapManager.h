#pragma once
#include <Core/Memory/Heap.h>

void TOS_InitHeapManager();
void TOS_RegisterHeap(TOS_Heap* heap);

size_t TOS_GetHeapCount();
TOS_Heap* TOS_GetHeaps();