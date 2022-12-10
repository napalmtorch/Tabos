#pragma once
#include <Lib/Types.h>

/// @brief Parse integer value from string @param str Pointer to string @return Integer values - returns 0 if failure occurs
int TOS_ConvertStrToInt(const char* str);

/// @brief Parse unsigned integer value from string @param str Pointer to string @return Unsigned integer value - returns 0 if failure occurs
uint32_t TOS_ConvertStrToUInt(const char* str);

/// @brief Parse hexadecimal integer value from string @param str Pointer to string @return Unsigned integer value - Undefined behavior if failure occurs
uint32_t TOS_ConvertHexToUInt(const char* str);

/// @brief Parse floating-point value from string @param str Pointer to string @return Floating-point value - returns 0 if failure occurs
float TOS_ConvertStrToFloat(const char* str);

/// @brief Generate string representation of integer value @param num Number to convert @param buff Output buffer @param base Number system to use @return Pointer to output buffer
char* TOS_ConvertIntToStr(int num, char* buff, int base);

/// @brief Generate string representation of unsigned integer value @param num Number to convert @param buff Output buffer @param base Number system to use @return Pointer to output buffer
char* TOS_ConvertUIntToStr(uint32_t num, char* buff, int base);

/// @brief Generate string representation of floating-point value @param num Number to convert @param buff Output buffer @param base Number system to use @return Pointer to output buffer
char* TOS_ConvertFloatToStr(float num, char* buff, int dot);

/// @brief Generate hexadecimal string representation of unsigned integer value @param num Number to convert @param buff Output buffer @param sz Number of bytes @return Pointer to output buffer
char* TOS_ConvertUIntToHex(uint32_t num, char* buff, uint8_t sz);
