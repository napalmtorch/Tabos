#include <Typecheck/Primitives/math/integer/i8.h>
#include <Typecheck/Primitives/math/integer/i16.h>
#include <Typecheck/Primitives/math/integer/i32.h>

#include <Typecheck/Primitives/math/float/f8.h>
#include <Typecheck/Primitives/math/float/f16.h>
#include <Typecheck/Primitives/math/float/f32.h>

#include <Typecheck/Primitives/math/boolean.h>

#include <Typecheck/type.h>

#include <TabosVM.h>

bool TVM_primitive_i8_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_I8; }
uint8_t * TVM_primitive_i8_apply(uint8_t *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_i8()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_I8,
        .check = TVM_primitive_i8_check,
        .apply = TVM_primitive_i8_apply
    };
}

bool TVM_primitive_i16_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_I16; }
uint16_t * TVM_primitive_i16_apply(uint16_t *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_i16()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_I16,
        .check = TVM_primitive_i16_check,
        .apply = TVM_primitive_i16_apply
    };
}

bool TVM_primitive_i32_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_I32; }
uint16_t * TVM_primitive_i32_apply(uint32_t *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_i32()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_I32,
        .check = TVM_primitive_i32_check,
        .apply = TVM_primitive_i32_apply
    };
}

// !!! TODO: @naparlmtorch ENABLE FLOATING POINT FLAG ON CPU (https://wiki.osdev.org/Floating_Point_Number) !!!

bool TVM_primitive_f8_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_F8; }
uint8_t * TVM_primitive_f8_apply(uint8_t *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_f8()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_F8,
        .check = TVM_primitive_f8_check,
        .apply = TVM_primitive_f8_apply
    };
}

bool TVM_primitive_f16_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_F16; }
uint16_t * TVM_primitive_f16_apply(uint16_t *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_f16()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_F16,
        .check = TVM_primitive_f16_check,
        .apply = TVM_primitive_f16_apply
    };
}

bool TVM_primitive_f32_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_F32; }
uint16_t * TVM_primitive_f32_apply(uint32_t *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_f32()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_F32,
        .check = TVM_primitive_f32_check,
        .apply = TVM_primitive_f32_apply
    };
}


bool TVM_primitive_boolean_check(TVM_type_t *type, void *data) { return type->identifier == __VM_TYPECHECK_PRMITIVE_boolean; }
bool * TVM_primitive_boolean_apply(bool *l, TVM_type_t *rt, void *r, TVM_operator_sign_t op)
{
    return l;
}
TVM_type_t TVM_primitive_boolean()
{
    return (TVM_type_t) {
        .identifier = __VM_TYPECHECK_PRMITIVE_boolean,
        .check = TVM_primitive_boolean_check,
        .apply = TVM_primitive_boolean_apply
    };
}