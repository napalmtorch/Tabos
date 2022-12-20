#ifndef __VM_TYPECHECK_TYPE
#define __VM_TYPECHECK_TYPE 1

#define TVM_type_t_NULLEXPR (TVM_type_t) { .identifier = -1 }

#if TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
#endif

typedef enum TVM_operator_sign {
    TVM_OPERATOR_SIGN_PLUS,
    TVM_OPERATOR_SIGN_MINUS,
    TVM_OPERATOR_SIGN_TIMES,
    TVM_OPERATOR_SIGN_OVER,
    TVM_OPERATOR_SIGN_AND,
    TVM_OPERATOR_SIGN_OR,
    TVM_OPERATOR_SIGN_XOR,
    TVM_OPERATOR_SIGN_NOT,
    TVM_OPERATOR_SIGN_EQUAL,
    TVM_OPERATOR_SIGN_NOT_EQUAL
} TVM_operator_sign_t;

typedef struct TVM_type {
    uint32_t identifier;

    bool   (*check) (struct TVM_type *, void *);
    void * (*apply) (void *, struct TVM_type *, void *, TVM_operator_sign_t);
} TVM_type_t;

#endif