#include <TabosVM.h>
#include <atd/list.h>
#include <atd/mem.h>

ATD_LINK_NODE(uintptr_t);
ATD_LIST_CREATE(uintptr_t, 0);

TVM_instruction_callback_t __TVM_INSTR_MAP__[65535] = { 0 };

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