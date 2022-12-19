#include <stdio.h>
#include <Typecheck/Primitives/math/integer/i8.h>
#include <Typecheck/Primitives/math/integer/i16.h>

int main() {
    TVM_type_t i8 = TVM_primitive_i8();
    TVM_type_t i8s = TVM_primitive_i8();
    TVM_type_t i16 = TVM_primitive_i16();

    printf("%s\n", i8.check(&i8s, NULL) ? "true" : "false");
    printf("%s\n", i16.check(&i8s, NULL) ? "true" : "false");
}