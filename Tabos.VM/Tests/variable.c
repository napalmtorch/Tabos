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
    "method\0" "type\0" "\xff"
    
    "BYC"
    "\xff" "\x00" "\x00"
    
    "EOE"
    "EOF";

int main() {
    TVM_module_t mod = TVM_modload(data);

    TVM_variable_t var = TVM_get_var_n(mod, "test");
    printf("var %s: %s\n", var.name, var.type);
}