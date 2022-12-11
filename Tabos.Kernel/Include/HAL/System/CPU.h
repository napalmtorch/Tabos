#pragma once
#include <Lib/Types.h>

static inline void TOS_DisableIRQs() { asm volatile("cli"); }

static inline void TOS_EnableIRQs() { asm volatile("sti"); }

static inline void TOS_Halt() { asm volatile("hlt"); }

static inline void TOS_FullHalt() { asm volatile("cli; hlt"); }