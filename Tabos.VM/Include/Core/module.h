#ifndef __VM_CORE_MODULE
#define __VM_CORE_MODULE 1

#ifdef TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdint.h>
    #include <stddef.h>
    #include <stdbool.h>
#endif

/// @brief required by TVM_modload to identify the module.
typedef struct TVM_manifest {
    size_t name_start;
    size_t name_end;

    size_t ver_start;
    size_t ver_end;
} TVM_manifest_t;
/// @brief required by TVM_exec to identify the module body
typedef struct TVM_body
{
    size_t vars_start;
    size_t vars_end;

    size_t code_start;
    size_t code_end;

    size_t consts_start;
    size_t consts_end;
} TVM_body_t;


/// @brief required by TVM_read to enclose code sections (functions declarations, bytecode)
typedef struct TVM_code
{
    size_t decl_start;
    size_t decl_end;

    size_t bytecode_start;
    size_t bytecode_end;
} TVM_code_t;
/// @brief required by TVM_get_var to enclose a variable
typedef struct TVM_variable
{
    char *name;
    char *type;
} TVM_variable_t;
/// @brief required by TVM_get_method to enclose a method
typedef struct TVM_method
{
    char *name;
    char *type;

    uint32_t offset;
} TVM_method_t;

/// @brief returned by TVM_modload.
typedef struct TVM_module {
    bool valid;

    TVM_manifest_t name;
    TVM_body_t body;

    const uint8_t *base;
} TVM_module_t;

#endif // __VM_CORE_MODULE