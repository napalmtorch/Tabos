#include <stdio.h>
#include <Typecheck/Primitives/math/integer/i8.h>
#include <Typecheck/Primitives/math/integer/i16.h>
#include <TabosVM.h>

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


int main() {
    TVM_module_t mod = TVM_modload(data);
    TVM_engine_processor_t proc = TVM_build(mod, TVM_read(mod));
    TVM_type_t i8 = TVM_primitive_i8();

    TVM_register_type(&proc, "sys.i8", &i8);

    TVM_type_t type = TVM_primitive_i8();
    printf("%s\n", type.check(TVM_resolve_typeexpr(&proc, "sys.i8"), NULL) ? "true" : "false");

    return 0;
}