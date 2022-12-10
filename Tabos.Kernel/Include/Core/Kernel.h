#pragma once
#include <Core/Multiboot.h>

/// @brief Start critical system components @param mbp Pointer to multiboot header
void TOS_KernelBoot(TOS_Multiboot* mbp);

/// @brief Main loop of kernel thread
void TOS_KernelRun();

/// @brief Get multiboot header @return Pointer to multiboot header
TOS_Multiboot* TOS_GetMultiboot();

/// @brief Clear entire VGA memory buffer
void TOS_FlushBootScreen();

/// @brief Get total amount of kernel ticks
uint64_t TOS_GetKernelTicks();

/// @brief Retrieve starting address of kernel memory
uintptr_t TOS_GetKernelStart();

/// @brief Retrieve end address of kernel memory
uintptr_t TOS_GetKernelEnd();

/// @brief Retrieve size of kernel memory
size_t TOS_GetKernelSize();

/// @brief Retrieve starting address/initial bottom of kernel stack
uintptr_t TOS_GetKernelStackBottom();

/// @brief Retrieve end address/initial top of kernel stack
uintptr_t TOS_GetKernelStackTop();

/// @brief Retrieve size of kernel stack
size_t TOS_GetKernelStackSize();