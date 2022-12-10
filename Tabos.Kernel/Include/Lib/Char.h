#pragma once
#include <Lib/Types.h>

/// @brief Check if character is of alpha-numerical type @param c Character to check @return Character is alpha-numeric
bool TOS_IsAlpha(char c);

/// @brief Check if character is a digit @param c Character to check @return Character is a digit
bool TOS_IsDigit(char c);

/// @brief Check if character is a hexadecimal digit @param c Character to check @return Character is hex
bool TOS_IsHexDigit(char c);

/// @brief Check if character is either of alpha-numerical type, or a digit @param c Character to check @return Character is alpha-numeric/hex
bool TOS_IsAlphaNum(char c);

/// @brief Check if character is a control character @param c Character to check @return Character is control
bool TOS_IsControl(char c);

/// @brief Check if character is a punctuation symbol @param c Character to check @return Character is punctual
bool TOS_IsPunctuation(char c);

/// @brief Check if character is a graphical character @param c Character to check @return Character is graphical
bool TOS_IsGraphical(char c);

/// @brief Check if alpha-numerical character is lowercase @param c Character to check @return Character is lowercase - returns false if character is not alpha-numerical
bool TOS_IsLower(char c);

/// @brief Check if alpha-numerical character is uppercase @param c Character to check @return Character is uppercase - returns false if character is not alpha-numerical
bool TOS_IsUpper(char c);

/// @brief Check if character is whitespace @param c Character to check @return Character is whitespace
bool TOS_IsSpace(char c);