#pragma once
#include <Lib/Types.h>

/// @brief Integer clamp
int TOS_Clamp(int v, int min, int max);

/// @brief Integer ABS
int TOS_Abs(int v);

/// @brief Floating-point ceiling
float TOS_CeilF(float v);

/// @brief Double ceiling
double TOS_CeilD(double v);

/// @brief Floating-point floor
float TOS_FloorF(float v);

/// @brief Double floor
double TOS_FloorD(double v);

/// @brief Floating-point power
float TOS_PowF(float x, int y);

/// @brief Double power
double OS_PowD(double x, int y);