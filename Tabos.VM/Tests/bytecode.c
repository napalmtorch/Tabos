#include <TabosVM.h>
#include <stdio.h>

const char data[] = 
    "TVM"
    "test.module\0" "1.0.0\0"
    
    "VAR"
    "varname\0" "number\0"
    "test\0" "string|null\0"
    "\xff"
    
    "EXE"
    "\xde"
    "\xff\xff\xff\xff" "testA\0" "number|void\0"
    "\x00\x00\x00\x00" "main\0" "string\0" 
    
    "BYC"
//      NOP        DBG        NOP        END
    "\x00\x00" "\xff\x00" "\x00\x00" "\xff\xff"
    
    "EOE"
    "EOF";

bool debug_instruction(TVM_engine_processor_t *proc)
{
    printf("DEBUG!\n");
    return true;
}

int main() {
    TVM_module_t mod = TVM_modload(data);
    TVM_code_t code = TVM_read(mod);

    TVM_register_bytecode(0xff, debug_instruction);

    TVM_engine_processor_t proc = TVM_build(mod, code);
    printf("IP: 0x%x\n", proc.IP);

    TVM_exec(&proc, false);
}