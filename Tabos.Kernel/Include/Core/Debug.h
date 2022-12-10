#pragma once
#include <Lib/Types.h>
#include <HAL/Devices/Serial.h>

#define DEBUG_INFO   "[  \x1b[36m>>\x1b[0m  ]"
#define DEBUG_OK     "[  \x1b[32mOK\x1b[0m  ]"
#define DEBUG_WARN   "[  \x1b[33m??\x1b[0m  ]"
#define DEBUG_ERROR  "[  \x1b[31m!!\x1b[0m  ]"
#define DEBUG_MALLOC "[\x1b[35mMALLOC\x1b[0m]"
#define DEBUG_FREE   "[ \x1b[33mFREE\x1b[0m ]"
#define DEBUG_MMAP   "[ \x1b[95mMMAP\x1b[0m ]"
#define DEBUG_MUNMAP "[\x1b[93mMUNMAP\x1b[0m]"
#define DEBUG_THREAD "[\x1b[95mTHREAD\x1b[0m]"
#define DEBUG_KILLED "[\x1b[93mKILLED\x1b[0m]"

void TOS_InitDebug();

/// @brief Output formatted string to debug log @param fmt Pointer to formatted string @param ... Arguments
void TOS_Log(const char* fmt, ...);

/// @brief Output formatted string to debug log and produce and error @param fmt Pointer to formatted string @param ... Arguments
void TOS_Error(const char* fmt, ...);

/// @brief Generate a kernel panic and output formatted string @param fmt Pointer to formatted string @param ... Arguments
void TOS_Panic(const char* fmt, ...);

/// @brief Generate breakpoint - currently only works with BOCHs
void TOS_Breakpoint();

TOS_SerialController* TOS_GetDebugSerial();