#include <TabosVM.h>
#include <stdbool.h>

TVM_variable_t TVM_VAR_PARSE(uintptr_t ptr)
{
    TVM_variable_t result = {0};

    result.name = (char *) ptr;
    result.type = (char *) (ptr + ATD_strlen(result.name) + 1);

    return result;
}
int TVM_VAR_LEN(uintptr_t ptr)
{
    size_t namesz = ATD_strlen((const char *) ptr);
    size_t typesz = ATD_strlen((const char *) ptr+namesz+1);

    return typesz+namesz;
}
TVM_variable_t TVM_VAR_AT(TVM_module_t m, size_t ind)
{
    TVM_variable_t result = {0};
    size_t i, x = 0;
    for (i = m.body.vars_start; i < m.body.vars_end; i++)
    {
        if (x == ind) return result = TVM_VAR_PARSE((uintptr_t)m.base + i);
        else i += TVM_VAR_LEN((uintptr_t)m.base + i) + 1, x++;
    }
    return result;
}

TVM_method_t TVM_METHOD_PARSE(uintptr_t ptr)
{
    TVM_method_t result = {0};

    result.offset = *(uint32_t*)ptr;
    result.name = (char *)ptr + 4;
    result.type = (char *)(ptr + 4 + ATD_strlen(result.name) + 1);

    return result;
}
int TVM_METHOD_LEN(uintptr_t ptr)
{
    ptr+=4;
    size_t namesz = ATD_strlen((const char *) ptr);
    size_t typesz = ATD_strlen((const char *) ptr+namesz+1);

    return namesz+typesz+4;
}
TVM_method_t TVM_METHOD_AT(TVM_module_t m, TVM_code_t c, size_t ind)
{
    TVM_method_t result = {0};
    size_t i, x = 0;
    for (i = m.body.code_start + c.decl_start; i < m.body.code_start + c.decl_end; i++)
    {
        if (x == ind) return result = TVM_METHOD_PARSE((uintptr_t)m.base + i);
        else i += TVM_METHOD_LEN((uintptr_t)m.base + i) + 1, x++;
    }
    return result;
}

TVM_module_t TVM_modload(const uint8_t data[])
{
    TVM_module_t result = {0};

    if (!TVM_validate(data)) return result;
    
    result.valid = true;
    result.base = data;

    int i = result.name.name_start = 3;
    while (data[i]) i++;
    result.name.name_end = i;

    result.name.ver_start = ++i;
    while (data[i]) i++;
    result.name.ver_end = i;

    // if there is no var section, jump ahead
    if (!VAR_validate((void*)((uintptr_t)data + ++i))) goto code;
    
    result.body.vars_start = i += 3;
    while (data[i] != 0xFF) i++;
    result.body.vars_end = i;

    code:
    // if there is no code section, return the module
    if (!EXE_validate((void*)((uintptr_t)data + ++i))) return result;

    result.body.code_start = i += 3;
    // continue until "end of exec" is reached
    while (!EOE_validate((void*)((uintptr_t)data + i))) i++;
    result.body.code_end = i;

    result.body.consts_start = i += 3;
    while (!EOF_validate((void*)((uintptr_t)data + i))) i++;
    result.body.consts_end = i;

    return result;
}
TVM_code_t TVM_read(TVM_module_t module)
{
    TVM_code_t result = { 0 };
    if (!module.valid) return result;

    const uint8_t *data = (const uint8_t *)((uintptr_t)module.base + module.body.code_start);

    // check if declaration section exists
    if (data[0] != 0xDE)
    {
        // if a bytecode section exists, skip ahead
        if (BYC_validate((void*)((uintptr_t)data))) goto bytecode;
        else return result; // otherwise, return result
    }

    int i = result.decl_start = 1;

    while (!BYC_validate((void*)((uintptr_t)data + i))) i++;
    result.decl_end = i;

    bytecode:
    result.bytecode_start = i+3;
    while (!EOE_validate((void*)((uintptr_t)data + i))) i++;
    result.bytecode_end = i;

    return result;
}

TVM_variable_t TVM_get_var_i(TVM_module_t module, size_t index) { return TVM_VAR_AT(module, index); }
TVM_variable_t TVM_get_var_n(TVM_module_t module, const char *name)
{
    int i = 0;
    TVM_variable_t current = TVM_VAR_AT(module, i);

    while (*current.name)
    {
        if (ATD_strcmp(current.name, name) == 0) return current;
        current = TVM_VAR_AT(module, ++i);
    }

    return current;
}
TVM_method_t TVM_get_method_i(TVM_module_t module, TVM_code_t code, size_t index) { return TVM_METHOD_AT(module, code, index); }
TVM_method_t TVM_get_method_n(TVM_module_t module, TVM_code_t code, const char *name)
{
    int i = 0;
    TVM_method_t current = TVM_METHOD_AT(module, code, i);

    while (*current.name)
    {
        if (ATD_strcmp(current.name, name) == 0) return current;
        current = TVM_METHOD_AT(module, code, ++i);
    }

    return current;
}