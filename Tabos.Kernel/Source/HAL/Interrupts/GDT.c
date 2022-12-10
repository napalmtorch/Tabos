#include <HAL/Interrupts/GDT.h>
#include <Core/Common.h>

#define KERNEL_CS_INDEX 1
#define KERNEL_DS_INDEX 2

extern void _gdt_flush(uint32_t value);

const TOS_GDTAccess KERNEL_CS_ACCESS = (TOS_GDTAccess)
{
    .accessed        = 0,
    .read_write      = 1,
    .dc_flag         = GDT_NONCONF,
    .executable      = true,
    .type            = 1,
    .privilege_level = 0,
    .present         = 1,
};

const TOS_GDTAccess KERNEL_DS_ACCESS = (TOS_GDTAccess)
{
    .accessed        = 0,
    .read_write      = 1,
    .dc_flag         = GDT_UPWARDS,
    .executable      = false,
    .type            = 1,
    .privilege_level = 0,
    .present         = 1,
};

const TOS_GDTFlags KERNEL_CS_FLAGS = (TOS_GDTFlags)
{
    .reserved    = 0,
    .long_mode   = false,
    .size        = GDT_32BIT,
    .granularity = GDT_ALIGN_4K,
};

const TOS_GDTFlags KERNEL_DS_FLAGS = (TOS_GDTFlags)
{
    .reserved    = 0,
    .long_mode   = false,
    .size        = GDT_32BIT,
    .granularity = GDT_ALIGN_4K,
};

static TOS_GDTRegister _reg                attr_align(0x100);
static TOS_GDTEntry    _entries[GDT_COUNT] attr_align(0x100);

void TOS_InitGDT()
{
    TOS_MemSet(_entries, 0, sizeof(_entries));
    _reg.base  = (uintptr_t)&_entries;
    _reg.limit = (uint16_t)((GDT_COUNT * sizeof(TOS_GDTEntry)) - 1);

    TOS_SetGDTDescriptor(0, 0, 0, (TOS_GDTAccess){ 0, 0, 0, 0, 0, 0, 0 }, (TOS_GDTFlags){ 0, 0, 0, 0 });
    TOS_SetGDTDescriptor(KERNEL_CS_INDEX, 0, 0xFFFFFFFF, KERNEL_CS_ACCESS, KERNEL_CS_FLAGS);
    TOS_SetGDTDescriptor(KERNEL_DS_INDEX, 0, 0xFFFFFFFF, KERNEL_DS_ACCESS, KERNEL_DS_FLAGS);
    _gdt_flush((uintptr_t)&_reg);

    TOS_Log("%s Initialized global descriptor table\n", DEBUG_OK);
}

void TOS_SetGDTDescriptor(uint8_t n, uint32_t base, uint32_t limit, TOS_GDTAccess access, TOS_GDTFlags flags)
{
    _entries[n].base_low    = base & 0xFFFF;
    _entries[n].base_middle = (base >> 16) & 0xFF;
    _entries[n].base_high   = (base >> 24) & 0xFF;

    _entries[n].limit_low  = limit & 0xFFFF;
    _entries[n].limit_high = (limit >> 16) & 0x0F;

    _entries[n].accessed        = access.accessed;
    _entries[n].read_write      = access.read_write;
    _entries[n].dc_flag         = access.dc_flag;
    _entries[n].executable      = access.executable;
    _entries[n].type            = access.type;
    _entries[n].privilege_level = access.privilege_level;
    _entries[n].present         = access.present;

    _entries[n].reserved    = 0;
    _entries[n].long_mode   = flags.long_mode;
    _entries[n].size        = flags.size;
    _entries[n].granularity = flags.granularity;

    TOS_Log("%s GDT[%d] - Base:%8x Limit:%8x Access:%2x Flags:%2x\n", DEBUG_INFO, n, base, limit, access, flags);
}