#pragma once
#include <Lib/Types.h>

/// @brief Align specified 32-bit number @param value Number to align @param alignment Value to align by @return Aligned value
uint32_t TOS_MemAlign(uint32_t value, uint32_t alignment);

/// @brief Align specified 64-bit number @param value Number to align @param alignment Value to align by @return Aligned value
uint64_t TOS_MemAlign64(uint64_t value, uint64_t alignment);

/// @brief Copy data in memory @param dest Pointer to destination @param src Pointer to source @param sz Size in bytes @return Pointer to destination
void* TOS_MemCopy(void* dest, const void* src, size_t sz);

/// @brief Fill data in memory with value @param ptr Pointer to memory location @param v Value to fill with @param sz Size in bytes @return Pointer to filled memory location
void* TOS_MemSet(void* ptr, int v, size_t sz);

/// @brief Fill data in memory with 16-bit value @param ptr Pointer to memory location @param v Value to fill with @param sz Size in bytes @return Pointer to filled memory location
void* TOS_MemSet16(void* ptr, uint16_t v, size_t sz);

/// @brief Compare 2 regions of memory @param ptr1 Pointer to first memory location @param ptr2 Pointer to second memory location @param sz Size in bytes @return Difference
int TOS_MemCompare(const void* ptr1, const void* ptr2, size_t sz);

/// @brief Locate the first occurrence of a value in a region of memory @param ptr Pointer to memory location @param v Value to locate @param sz Size in bytes @return Pointer to first occurrence
const void* TOS_MemLocate(const void* ptr, int v, size_t sz);

/// @brief Allocate a block of memory to the heap @param sz Size in bytes @return Pointer to allocated memory
void* TOS_Alloc(size_t sz);

/// @brief Free an allocated block of memory from the heap @param ptr Pointer to allocated memory 
void TOS_Free(void* ptr);

void TOS_FreeArray(void** ptr, size_t count);

