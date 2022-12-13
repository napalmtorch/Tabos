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
    "\xff\xfa\xfb\x00" "testB\0" "string\0" 
    
    "BYC"
    "\0" "\0" "\0"
    
    "EOE"
    "EOF";

int main() {
    TVM_module_t mod = TVM_modload(data);

    TVM_method_t method = TVM_get_method_n(mod, TVM_read(mod), "testB");
    printf("method %s: %s at %x\n", method.name, method.type, method.offset);
}