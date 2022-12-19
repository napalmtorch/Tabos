#ifndef __VM_TYPECHECK_PRMITIVE_F8
#define __VM_TYPECHECK_PRMITIVE_F8 3

#if TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
#endif

#include <Typecheck/type.h>

TVM_type_t TVM_primitive_f8();

#endif