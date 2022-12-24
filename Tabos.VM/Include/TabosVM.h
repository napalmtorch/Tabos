#ifndef __TABOS_VM_H
#define __TABOS_VM_H 1

#if TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
#endif

#include <Core/module.h>
#include <Runtime/engine.h>
#include <Typecheck/type.h>

#include <atd/mem.h>
#include <atd/io.h>

static inline bool TVM_validate(const void *data) { return ATD_memcmp(data, "TVM", 3) == 0; }
static inline bool VAR_validate(const void *data) { return ATD_memcmp(data, "VAR", 3) == 0; }
static inline bool EXE_validate(const void *data) { return ATD_memcmp(data, "EXE\xde", 4) == 0; }
static inline bool EOE_validate(const void *data) { return ATD_memcmp(data, "EOE", 3) == 0; }
static inline bool EOF_validate(const void *data) { return ATD_memcmp(data, "EOF", 3) == 0; }
static inline bool BYC_validate(const void *data) { return ATD_memcmp(data, "BYC", 3) == 0; }

/// @brief load a module from byte content @param data data to be read into the module @returns a valid module if the reading was successful, invalid if it was not.
TVM_module_t TVM_modload(const uint8_t data[]);

/// @brief load the code section from a module @param module the module to read from @returns a code section @exception undefined behaviour if the code section is not found
TVM_code_t TVM_read(TVM_module_t module);

/// @brief get the variable info of a specific index @param module the module @param index index to look at @returns the variable info @exception undefined behaviour if the index is out of range
TVM_variable_t TVM_get_var_i(TVM_module_t module, size_t index);

/// @brief get the method info of a specific index @param module the module @param index index to look at @returns the method info @exception undefined behaviour if the index is out of range
TVM_method_t TVM_get_method_i(TVM_module_t module, TVM_code_t code, size_t index);

/// @brief get variable info by variable name @param module the module @param name variable name @returns the variable info @exception undefined behaviour if the variable name is not found
TVM_variable_t TVM_get_var_n(TVM_module_t module, const char *name);

/// @brief get method info by method name @param module the module @param name method name @returns the method info @exception undefined behaviour if the method name is not found
TVM_method_t TVM_get_method_n(TVM_module_t module, TVM_code_t code, const char *name);

/// @brief map a bytecode with the given instruction callback @param code the bytecode @param instruction the instruction to associate the bytecode with
void TVM_register_bytecode(uint16_t code, TVM_instruction_callback_t instruction);

/// @brief execute the bytecode @param processor the processor derived from module and code @param runOne run one bytecode and return @return true if the execution should continue or false if it shouldn't
bool TVM_exec(TVM_engine_processor_t *processor, bool runOne);

/// @brief build the vm processor @param module the module @param code the parsed code from the module @return a new processor to execute bytecode
TVM_engine_processor_t TVM_build(TVM_module_t module, TVM_code_t code);

bool TVM_register_type(TVM_engine_processor_t *processor, const char *name, TVM_type_t *type);

TVM_type_t * TVM_resolve_typeexpr(TVM_engine_processor_t *processor, const char *expr);

/// @brief init TVM base instructions
void TVM_init();

#endif