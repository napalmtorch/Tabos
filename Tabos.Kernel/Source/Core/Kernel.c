#include <Core/Common.h>

extern uintptr_t _kernel_start;
extern uintptr_t _kernel_end;
extern uintptr_t _stack_top;
extern uintptr_t _stack_bottom;

static TOS_Multiboot* _mboot;
static uint64_t      _ticks;
static TOS_RAMFS _test_ramfs;

const char vm_data[] = 
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

bool vm_debug_instruction(TVM_engine_processor_t *proc)
{
    ATD_printf("DEBUG!\n");
    return true;
}

int vm_main(TOS_PtrList* args) 
{
    TVM_module_t mod = TVM_modload(vm_data);
    TVM_code_t code = TVM_read(mod);

    TVM_register_bytecode(0xff, vm_debug_instruction);

    TVM_engine_processor_t proc = TVM_build(mod, code);
    ATD_printf("IP: 0x%x\n", proc.IP);

    TVM_exec(&proc, false);
    return 420;
}

void TOS_KernelBoot(TOS_Multiboot* mbp)
{
    // set local multiboot pointer so we can access it after we leave this function
    _mboot = mbp;

    // reset tick count
    _ticks = 0;

    // get the system ready enough for debugging output
    TOS_FlushBootScreen();
    TOS_FlushPrintBuffers();
    TOS_InitDebug();
    TOS_Log("%s Booting Tabos Kernel...\n", DEBUG_INFO);

    // parse multiboot memory map and passover to our own memory map
    TOS_InitMemMgr(true);

    // initalize kernel heap - for the moment it uses all available memory left at this point
    TOS_InitHeapManager();

    // initialize descriptor tables and interrupts
    TOS_InitGDT();
    TOS_InitIDT();

    // initialize multitasking and create kernel thread
    TOS_InitScheduler();
    TOS_InitKernelThread();
    
    TOS_Thread* vm = TOS_NewThread("test_vm", 0x10000, vm_main, THREAD_PRIORITY_NORMAL, TOS_NewPtrList());
    TOS_LoadThread(vm);
    TOS_StartThread(vm);   

    // initialize device/driver manager
    TOS_InitDriverManager();

    // assume our ramdisk always exists and is the first module - then load it
    TOS_MemoryBlock* mod = TOS_GetMemBlockByType(MEM_MODULE);
    if (mod != NULL && mod->type == MEM_MODULE) { TOS_InitRAMFS(&_test_ramfs, (void*)mod->addr, mod->sz); }
}

void TOS_KernelRun()
{
    TOS_Log("%s Entered kernel main\n", DEBUG_OK);

    uint32_t last = 0, fps = 0, frames = 0;
    while (true)
    {
        TOS_RealTimeClock* rtc = (TOS_RealTimeClock*)TOS_FetchDriverFromName("RTC");
        if (rtc != NULL)
        {
            frames++;
            uint32_t secs = (uint32_t)rtc->seconds;
            if (last != secs)
            {
                last = secs;
                fps = frames;
                frames = 0;
                TOS_Log("FPS: %u\n", fps);
            }
        }

        TOS_SwitchThread(true);   
    }
}

void TOS_FlushBootScreen()
{
    TOS_MemSet16((void*)0xB8000, 0x0F20, 80 * 25 * 2);
}

TOS_Multiboot* TOS_GetMultiboot() { return _mboot; }

uint64_t TOS_GetKernelTicks() { return _ticks; }

uintptr_t TOS_GetKernelStart() { return (uintptr_t)&_kernel_start; }

uintptr_t TOS_GetKernelEnd() { return (uintptr_t)&_kernel_end; }

size_t TOS_GetKernelSize() { return TOS_GetKernelEnd() - TOS_GetKernelStart(); }

uintptr_t TOS_GetKernelStackBottom() { return (uintptr_t)&_stack_bottom; }

uintptr_t TOS_GetKernelStackTop() { return (uintptr_t)&_stack_top; }

size_t TOS_GetKernelStackSize() { return TOS_GetKernelStackTop() - TOS_GetKernelStackBottom(); }