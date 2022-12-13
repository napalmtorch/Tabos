#ifndef __TABOS_VM_H
#define __TABOS_VM_H 1

#include <Core/module.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <atd/memory.h>
#include <atd/io.h>

static inline bool TVM_validate(const void *data) { return ATD_memcmp(data, "TVM", 3) == 0; }
static inline bool VAR_validate(const void *data) { return ATD_memcmp(data, "VAR", 3) == 0; }
static inline bool EXE_validate(const void *data) { return ATD_memcmp(data, "EXE", 3) == 0; }
static inline bool EOE_validate(const void *data) { return ATD_memcmp(data, "EOE", 3) == 0; }
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

#endif