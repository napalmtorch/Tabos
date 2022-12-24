#ifndef __VM_RUNTIME_ENGINE
#define __VM_RUNTIME_ENGINE 1
#include <Core/module.h>

/// @brief required by the vm to track execution flow
typedef struct TVM_engine_processor {
    TVM_module_t module;
    TVM_code_t code;
    uint32_t IP;

    uint8_t R0;
    uint16_t R1;
    uint32_t R2;

    uint32_t GPR;
    
    uintptr_t type_names_head;
    uintptr_t types_head;

    uintptr_t vars_head;
    uintptr_t methods_head;

    uint32_t call_stack;

} TVM_engine_processor_t;

/// @brief required by TVM_exec and TVM_register_bytecode @param processor the vm's processor
typedef bool (*TVM_instruction_callback_t) (TVM_engine_processor_t *processor);

inline static uint8_t TVM_ENGINE_PROCESSOR_CYCLE8(TVM_engine_processor_t *this)
{
    const uint8_t *base = (uint8_t *)((uintptr_t)this->module.base + (this->module.body.code_start + this->code.bytecode_start));
    return base[this->IP++];
}
inline static uint16_t TVM_ENGINE_PROCESSOR_CYCLE16(TVM_engine_processor_t *this)
{
    uint16_t result = *(uint16_t *)((uintptr_t)this->module.base + (this->module.body.code_start + this->code.bytecode_start) + this->IP);
    this->IP += sizeof(uint16_t);
    return result;
}
inline static uint32_t TVM_ENGINE_PROCESSOR_CYCLE32(TVM_engine_processor_t *this)
{
    uint32_t result = *(uint32_t *)((uintptr_t)this->module.base + (this->module.body.code_start + this->code.bytecode_start) + this->IP);
    this->IP += sizeof(uint32_t);
    return result;
}
inline static uint64_t TVM_ENGINE_PROCESSOR_CYCLE64(TVM_engine_processor_t *this)
{
    uint64_t result = *(uint64_t *)((uintptr_t)this->module.base + (this->module.body.code_start + this->code.bytecode_start) + this->IP);
    this->IP += sizeof(uint64_t);
    return result;
}

#endif