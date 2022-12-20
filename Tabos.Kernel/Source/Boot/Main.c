#include <Core/Common.h>

void main(TOS_Multiboot* mbp)
{
    THIS_THREAD = NULL;
    NEXT_THREAD = NULL;

    TOS_KernelBoot(mbp);
    TOS_ToggleScheduler(true);
    TOS_KernelRun();
}