#pragma once
#include <Lib/Types.h>

#define IDT_COUNT 256

typedef enum
{
    /// @brief Programmable interval timer
    IRQ0 = 32,
    /// @brief PS/2 Keyboard
    IRQ1,
    /// @brief Cascade
    IRQ2,
    /// @brief Serial COM2 (if enabled)
    IRQ3,
    /// @brief Serial COM1 (if enabled)
    IRQ4,
    /// @brief LPT2 (if enabled)
    IRQ5,
    /// @brief Floppy disk
    IRQ6,
    /// @brief LPT1 (unreliable)
    IRQ7,
    /// @brief CMOS real-time clock (if enabled)
    IRQ8,
    /// @brief Free for peripherals/legacy SCSI/NIC
    IRQ9,
    /// @brief Free for peripherals/SCSI/NIC
    IRQ10,
    /// @brief Free for peripherals/SCSI/NIC
    IRQ11,
    /// @brief PS/2 Mouse
    IRQ12,
    /// @brief FPU/Coprocessor/Inter-processor
    IRQ13,
    /// @brief Primary ATA hard disk
    IRQ14,
    /// @brief Secondary ATA hard disk
    IRQ15,
    /// @brief System call
    IRQ_SYSCALL = 160,
} IRQ;

typedef enum 
{
    IDT_TASK   = 0x05,
    IDT_INT16  = 0x06,
    IDT_INT32  = 0x0E,
    IDT_TRAP16 = 0x07,
    IDT_TRAP32 = 0x0F,
} IDT_TYPE;

typedef struct
{
    uint8_t  type    : 4;
    bool     unused  : 1;
    uint8_t  dpl     : 2;
    bool     present : 1;
} attr_pack TOS_IDTFlags;

typedef struct
{
    uint16_t    offset_low;
    uint16_t    segment;
    uint8_t     reserved;
    TOS_IDTFlags flags;
    uint16_t    offset_high;
} attr_pack TOS_IDTEntry;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} attr_pack TOS_IDTRegister;

typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t irq, error;
    uint32_t eip, cs, eflags, useresp, ss;
} attr_pack TOS_IRQContext;

typedef void (*TOS_IRQProtocol)(TOS_IRQContext* context);

void TOS_InitIDT();
void TOS_SetIDTDescriptor(uint8_t n, uint32_t base, TOS_IDTFlags flags, uint16_t seg);
void TOS_RegisterIRQ(IRQ irq, TOS_IRQProtocol protocol);
void TOS_UnregisterIRQ(IRQ irq);
void TOS_AcknowledgeIRQ(TOS_IRQContext* context);
bool TOS_IsInterruptsEnabled();