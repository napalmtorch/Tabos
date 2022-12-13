#include <TabosVM.h>

TVM_instruction_callback_t __TVM_INSTR_MAP__[65535] = { 0 };

void TVM_register_bytecode(uint16_t b, TVM_instruction_callback_t instr) { __TVM_INSTR_MAP__[b] = instr; }

TVM_engine_processor_t TVM_build(TVM_module_t m, TVM_code_t c)
{
    TVM_engine_processor_t result = {
        .module = m,
        .code = c,
        .IP = TVM_get_method_n(m, c, "main").offset
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

    ATD_printf("%d\n", byc);

    // end instruction
    if (byc == 0xffff) return false;

    if (callback) result = callback(proc);
    
    if ((!result && byc) || runOne) return result;
    goto __$TVM_EXEC_LOOP;
}