#pragma once
#include <Lib/Types.h>

/// @brief Clear the working buffers used for primary print functions
void TOS_FlushPrintBuffers();

/// @brief Print list of arguments @param fmt Pointer to formatted string @param args Variadic arguments
void TOS_PrintArgs(const char* fmt, va_list args);

/// @brief Print list of arguments @param fmt Pointer to formatted string @param ... Arguments
void TOS_Print(const char* fmt, ...);

/// @brief Print list of arguments to specified location @param buff Pointer to output buffer @param fmt Pointer to formatted string @param args Variadic arguments
void TOS_PrintArgsTo(char* buff, const char* fmt, va_list args);

/// @brief Print list of arguments to specified location @param buff Pointer to output buffer @param fmt Pointer to formatted string @param ... Arguments
void TOS_PrintTo(char* buff, const char* fmt, ...);