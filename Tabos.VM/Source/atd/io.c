#include <atd/io.h>
#include <flags.h>

#ifdef TABOS_KERNEL
    #include <Core/Common.h>
#else
    #include <stdio.h>
    #include <stdarg.h>
#endif


int ATD_printf(const char *fmt, ...)
{  
    #ifndef __TVM_PROD__
        va_list _list;
        va_start(_list, fmt);  
        int r = 0;
        #ifdef TABOS_KERNEL
            r = TOS_PrintArgs(fmt, _list);
        #else
            r = vprintf(fmt, _list);
        #endif
        va_end(_list);
        return r;
    #endif
}