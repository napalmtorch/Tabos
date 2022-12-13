#include <TabosVM.h>
#include <stdio.h>

const char data[] = "TVMtest.module\0""1.0.0\0""VARvarname\0""number\0""test\0""string|null\0\xff""EXE""\xde""method\0""type\0""BYC""\0""\0""\0""EOEEOF";

int main() {
    TVM_module_t mod = TVM_modload(data);
    printf("base: %d, valid: %s\n\
    name_start: %d, name_end: %d\n\
    ver_start: %d, ver_end: %d\n\
    vars_start: %d, vars_end: %d\n\
    code_start: %d, code_end: %d\n",
        mod.base,
        mod.valid ? "true": "false",
        mod.name.name_start, mod.name.name_end,
        mod.name.ver_start, mod.name.ver_end,
        mod.body.vars_start, mod.body.vars_end,
        mod.body.code_start, mod.body.code_end
    );

    TVM_code_t code = TVM_read(mod);
    printf(
        "\tdecl_start: %d, decl_end: %d\n"
        "\tbytecode_start: %d, bytecode_end: %d\n",
        code.decl_start, code.decl_end,
        code.bytecode_start, code.bytecode_end
    );
}