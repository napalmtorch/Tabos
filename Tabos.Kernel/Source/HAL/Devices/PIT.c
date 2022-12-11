#include <HAL/Devices/PIT.h>
#include <Core/Common.h>

#define PIT_DIVISOR 1193180
#define PIT_CMD     0x43
#define PIT_DATA    0x40

bool TOS_StartPIT(TOS_PITTimer* pit, uint32_t freq)
{
    pit->ticks        = 0;
    pit->tps          = 0;
    pit->timer        = 0;
    pit->millis       = 0;
    pit->millis_total = 0;
    pit->freq         = TOS_Clamp(freq, 30, 5000);

    bool ints = TOS_IsInterruptsEnabled();
    TOS_DisableIRQs();

    uint32_t f = PIT_DIVISOR / pit->freq;
    uint8_t  h = (uint8_t)((f & 0xFF00) >> 8);
    uint8_t  l = (uint8_t)((f & 0x00FF));

    TOS_PortOutb(PIT_CMD, 0x36);
    TOS_PortOutb(PIT_DATA, l);
    TOS_PortOutb(PIT_DATA, h);

    TOS_RegisterIRQ(IRQ0, TOS_HandleInterruptPIT);
    if (ints) { TOS_EnableIRQs(); }
    return true;
}

void TOS_StopPIT(TOS_PITTimer* pit)
{   
    TOS_UnregisterIRQ(IRQ0);
}

void TOS_HandleInterruptPIT(TOS_IRQContext* context)
{
    TOS_AcknowledgeIRQ(context);
    static TOS_PITTimer* pit;
    if (pit == NULL) { pit = (TOS_PITTimer*)TOS_FetchDriverFromName("PIT"); }
    if (pit == NULL) { return; }

    pit->ticks++;
    pit->timer++;

    if (pit->timer >= (pit->freq / 1000))
    {
        pit->timer = 0;
        pit->millis++;
        pit->millis_total++;
    }

    if (pit->millis > 1000) { pit->millis = 0; }
}
