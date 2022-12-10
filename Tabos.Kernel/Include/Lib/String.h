#pragma once
#include <Lib/Types.h>

/// @brief Allocate a new string @param str Pointer to source string @return Pointer to newly allocated string
char* TOS_NewString(const char* str);

/// @brief Get length of specified string @param str Pointer to string @return Size of string in bytes - returns 0 if failure
size_t TOS_StrLen(const char* str);

/// @brief Concatenate two strings @param str Pointer to destination string @param append Pointer to string thats appended to destination @return Pointer to destination string
char* TOS_StrConcat(char* str, const char* append);

/// @brief Append a character to a string @param str Pointer to destination string @param c Character thats appended to destination @return Pointer to destination string
char* TOS_StrAppend(char* str, char c);

/// @brief Compare two strings @param str1 Pointer to first string @param str2 Pointer to second string @return Difference
int TOS_StrCompare(const char* str1, const char* str2);

/// @brief Check if two strings are equal @param str1 Pointer to first string @param str2 Pointer to second string @return Strings are equal
bool TOS_StrEquals(const char* str1, const char* str2);

char* TOS_StrCopy(char* dest, const char* src);

char* TOS_StrRChr(const char* str, char c);

char** TOS_StrSplit(const char* str, char delim, size_t* count);