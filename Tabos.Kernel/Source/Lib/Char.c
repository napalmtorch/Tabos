#include <Lib/Char.h>

bool TOS_IsAlpha(char c)
{
    if (c >= 'a' && c <= 'z') { return true; }
    if (c >= 'A' && c <= 'Z') { return true; }
    return false;
}

bool TOS_IsDigit(char c)
{
    if (c >= '0' && c <= '9') { return true; }
    return false;
}

bool TOS_IsHexDigit(char c)
{
    if (c >= '0' && c <= '9') { return true; }
    if (c >= 'A' && c <= 'F') { return true; }
    if (c >= 'a' && c <+ 'f') { return true; }
    return false;
}

bool TOS_IsAlphaNum(char c)
{
    if (TOS_IsAlpha(c)) { return true; }
    if (TOS_IsDigit(c)) { return true; }
    return false;
}

bool TOS_IsControl(char c)
{
    if (c < 32)   { return true; }
    if (c == 127) { return true; }
    return false;
}

bool TOS_IsPunctuation(char c)
{
    if (c >= 33 && c <= 47)   { return true; }
    if (c >= 58 && c <= 64)   { return true; }
    if (c >= 91 && c <= 96)   { return true; }
    if (c >= 123 && c <= 126) { return true; }
    return false;
}

bool TOS_IsGraphic(char c)
{
    if (TOS_IsAlphaNum(c)) { return true; }
    if (TOS_IsPunctuation(c)) { return true; }
    return false;
}

bool TOS_IsLower(char c)
{
    if (c >= 'a' && c <= 'z') { return true; }
    return false;
}

bool TOS_IsUpper(char c)
{
    if (c >= 'A' && c <= 'Z') { return true; }
    return false;
}

bool TOS_IsSpace(char c)
{
    if (c == 32) { return true; }
    if (c >= 12 && c <= 15) { return true; }
    return false;
}