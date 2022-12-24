#include <TabosVM.h>
#include <atd/list.h>
#include <atd/mem.h>

ATD_LINK_NODE(uintptr_t);
ATD_LIST_CREATE(uintptr_t, 0);

#pragma region INSTRUCTIONS

#pragma region NULL_INSTR
bool NULL_OPERATION(TVM_engine_processor_t *proc)
{
    return true;
}
#pragma endregion
#pragma region FLOW_INSTR
#pragma region CALL
bool CALLMS(TVM_engine_processor_t *proc)
{
    const char *IDENTIFIER = (proc->module.base + proc->module.body.code_start + proc->code.bytecode_start) + proc->IP;
    TVM_method_t method = TVM_get_method_n(proc->module, proc->code, IDENTIFIER);

    proc->IP += ATD_strlen(IDENTIFIER) + 1;
    ATD_printf("%s: %X\n", method.name, method.offset);

    return true;
}
bool CALLM8(TVM_engine_processor_t *proc)
{
    uint8_t IDENTIFIER = TVM_ENGINE_PROCESSOR_CYCLE8(proc);
    TVM_method_t method = TVM_get_method_i(proc->module, proc->code, IDENTIFIER);

    ATD_printf("%s: %X\n", method.name, method.offset);

    return true;
}
bool CALLM16(TVM_engine_processor_t *proc)
{
    uint16_t IDENTIFIER = TVM_ENGINE_PROCESSOR_CYCLE16(proc);
    TVM_method_t method = TVM_get_method_i(proc->module, proc->code, IDENTIFIER);

    ATD_printf("%s: %X\n", method.name, method.offset);

    return true;
}
bool CALLM32(TVM_engine_processor_t *proc)
{
    uint32_t IDENTIFIER = TVM_ENGINE_PROCESSOR_CYCLE32(proc);
    TVM_method_t method = TVM_get_method_i(proc->module, proc->code, IDENTIFIER);

    ATD_printf("%s: %X\n", method.name, method.offset);

    return true;
}
#pragma endregion
#pragma endregion

#pragma endregion

TVM_instruction_callback_t __TVM_INSTR_MAP__[65535] = { 0 };
bool __TVM_INSTR_MAP_INIT__ = false;

void TVM_register_bytecode(uint16_t b, TVM_instruction_callback_t instr) { __TVM_INSTR_MAP__[b] = instr; }

TVM_engine_processor_t TVM_build(TVM_module_t m, TVM_code_t c)
{
    TVM_engine_processor_t result = {
        .module = m,
        .code = c,
        .IP = TVM_get_method_n(m, c, "main").offset,
        .type_names_head = ATD_LIST_uintptr_t_MAKE(),
        .types_head = ATD_LIST_uintptr_t_MAKE()
    };
    return result;
}

void TVM_init()
{
    if (__TVM_INSTR_MAP_INIT__) return;

    TVM_register_bytecode(0, NULL_OPERATION);
    TVM_register_bytecode(1, CALLMS);
    TVM_register_bytecode(2, CALLM8);
    TVM_register_bytecode(3, CALLM16);
    TVM_register_bytecode(4, CALLM32);

    __TVM_INSTR_MAP_INIT__ = true;
}

bool TVM_exec(TVM_engine_processor_t *proc, bool runOne)
{
    // inits
    bool result = false;
    TVM_instruction_callback_t callback = NULL;
    uint16_t byc = 0;

    // loop
    __$TVM_EXEC_LOOP:
    
    // get the bytecode
    byc = TVM_ENGINE_PROCESSOR_CYCLE16(proc);
    callback = __TVM_INSTR_MAP__[byc];

    // end instruction
    if (byc == 0xffff) return false;

    if (callback) result = callback(proc);
    
    if ((!result && byc) || runOne) return result;
    goto __$TVM_EXEC_LOOP;
}

bool TVM_register_type(TVM_engine_processor_t *proc, const char *name, TVM_type_t *type)
{
    *ATD_LIST_uintptr_t_PUSH(proc->type_names_head) = name;
    *ATD_LIST_uintptr_t_PUSH(proc->types_head) = type;
    return true;
}
TVM_type_t * TVM_resolve_typeexpr(TVM_engine_processor_t *processor, const char *expr)
{
    size_t i, sz = ATD_LIST_uintptr_t_LENGTH(processor->type_names_head);
    ATD_printf("%d\n", sz);
    for (i = 1; i < sz; i++)
    {
        if (ATD_strcmp(((char*)*ATD_LIST_uintptr_t_AT(processor->type_names_head, i)), expr) == 0)
            return ((TVM_type_t *)*ATD_LIST_uintptr_t_AT(processor->types_head, i));
    }

    return NULL;
}