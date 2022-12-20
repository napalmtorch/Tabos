#pragma once
#include <Lib/Types.h>
#include <Lib/PointerList.h>
#include <Core/Memory/MemoryManager.h>
#include <Core/Memory/Heap.h>

void TOS_StartGarbageCollector();
void TOS_GarbageCollect(bool notify);
