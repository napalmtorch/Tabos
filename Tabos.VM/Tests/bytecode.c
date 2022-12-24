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
    "\xff\xff\x00\x00" "testA\0" "number|void\0"
    "\x00\x00\x00\x00" "main\0" "void\0" 
    "\xff\x00\xff\x00" "test.method\0" "void\0"
    "\x00\xff\x00\xff" "ok\0" "number\0"
    
    "BYC"
//    callms    test.method
    "\x01\x00" "test.method\0"
//    callm8    0
    "\x02\x00" "\0"
//    callm16   1
    "\x03\x00" "\x01\0"
//    callm32   2
    "\x04\x00" "\x03\0\0\0"
//     end
    "\xff\xff"
    
    "EOE"
    "EOF";

int main() {
    TVM_init();

    TVM_module_t mod = TVM_modload(data);
    TVM_code_t code = TVM_read(mod);

    TVM_engine_processor_t proc = TVM_build(mod, code);
    TVM_exec(&proc, false);
}