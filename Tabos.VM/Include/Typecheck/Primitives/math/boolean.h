#ifndef __VM_TYPECHECK_PRMITIVE_boolean
#define __VM_TYPECHECK_PRMITIVE_boolean 6

#if TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
#endif

#include <Typecheck/type.h>

TVM_type_t TVM_primitive_boolean();

#endif