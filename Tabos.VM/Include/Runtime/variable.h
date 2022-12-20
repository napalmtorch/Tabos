#ifndef __VM_RUNTIME_VARIABLE
#define __VM_RUNTIME_VARIABLE 1

#if TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
#endif

#include <Typecheck/type.h>

typedef struct TVM_engine_var {
    const char *name;
    TVM_type_t type;

    uintptr_t pointer;
} TVM_engine_var_t;

#endif