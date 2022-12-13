#include <atd/mem.h>

#ifdef TABOS_KERNEL
    #include <Core/Common.h>
#else
    #include <stdio.h>
    #include <stdarg.h>
#endif

int ATD_memcmp(const void *left, const void *right, size_t sz)
{
    #ifdef TABOS_KERNEL
        return TOS_MemCompare(left, right, sz);
    #else
        return memcmp(left, right, sz);
    #endif
}
int ATD_strcmp(const char *left, const char *right)
{
    #ifdef TABOS_KERNEL
        return TOS_StrCompare(left, right);
    #else 
        return strcmp(left, right);
    #endif
}
size_t ATD_strlen(const char *str)
{
    #ifdef TABOS_KERNEL
        return TOS_StrLen(str);
    #else 
        return strlen(str);
    #endif
}