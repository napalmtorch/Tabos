#include <atd/memory.h>

#ifdef TABOS_KERNEL

// put here your wrappers

#else
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

int ATD_memcmp(const void *left, const void *right, size_t sz)
{
    return memcmp(left, right, sz);
}
int ATD_strcmp(const char *left, const char *right)
{
    return strcmp(left, right);
}
size_t ATD_strlen(const char *str)
{
    return strlen(str);
}

#endif