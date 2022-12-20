#include <HAL/Devices/PIT.h>
#include <Core/Common.h>

#define PIT_DIVISOR 1193182
#define PIT_CMD     0x43
#define PIT_DATA    0x40

static TOS_PITTimer* _pit;

bool TOS_StartPIT(TOS_PITTimer* pit, uint32_t freq)
{
    _pit              = pit;
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
    if (_pit != NULL)
    {
        _pit->ticks++;
        _pit->timer++;

        if (_pit->timer >= (_pit->freq / 1000))
        {
            _pit->timer = 0;
            _pit->millis++;
            _pit->millis_total++;
        }

        if (_pit->millis > 1000) { _pit->millis = 0; }
    }
    
    if (THIS_THREAD != NULL) { TOS_UpdateThreadTime(THIS_THREAD); }
    TOS_AcknowledgeIRQ(context);
}
