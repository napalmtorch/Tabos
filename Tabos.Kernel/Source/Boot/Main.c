#include <Core/Common.h>

void main(TOS_Multiboot* mbp)
{
    TOS_KernelBoot(mbp);
    TOS_ToggleScheduler(true);
    TOS_KernelRun();
}