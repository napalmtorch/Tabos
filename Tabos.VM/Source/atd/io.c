#include <atd/io.h>

#ifdef TABOS_KERNEL

// put here your wrappers

#else
#include <stdio.h>
#include <stdarg.h>

int ATD_printf(const char *fmt, ...)
{
    va_list _list;
    va_start(_list, fmt);
    int r = vprintf(fmt, _list);
    va_end(_list);
    return r;
}

#endif