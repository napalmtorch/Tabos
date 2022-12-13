#include <HAL/Devices/RTC.h>
#include <Core/Common.h>

#define RTC_CMD  0x70
#define RTC_DATA 0x71

const double RTC_MILLIS_TIME = 0.9765625;

static TOS_RealTimeClock* _rtc;

bool TOS_StartRTC(TOS_RealTimeClock* rtc, void* unused)
{
    _rtc              = rtc;
    rtc->tm           = TOS_NewDateTime(0, 0, 0, 0, 0, 0, 0, 0);
    rtc->ticks        = 0;
    rtc->tps          = 0;
    rtc->timer        = 0;
    rtc->millis       = 0;
    rtc->millis_total = 0;

    bool ints = TOS_IsInterruptsEnabled();
    TOS_DisableIRQs();

    TOS_PortOutb(RTC_CMD, 0x8B);
    uint8_t data = TOS_PortInb(RTC_DATA);
    TOS_PortOutb(RTC_CMD, 0x8B);
    TOS_PortOutb(RTC_DATA, data | 0x40);
    TOS_PortOutb(RTC_CMD, 0x0C);
    TOS_PortInb(RTC_DATA);

    TOS_RegisterIRQ(IRQ8, TOS_HandleInterruptRTC);

    if (ints) { TOS_EnableIRQs(); }
    return true;
}

void TOS_StopRTC(TOS_RealTimeClock* rtc)
{
    TOS_UnregisterIRQ(IRQ8);
    TOS_PortOutb(RTC_CMD, 0x0C);
    TOS_PortInb(RTC_DATA);
}

void TOS_UpdateRTC()
{
    _rtc->tm.second = TOS_ReadRTCRegister(0x00);
    _rtc->tm.minute = TOS_ReadRTCRegister(0x02);
    _rtc->tm.hour   = TOS_ReadRTCRegister(0x04);
    _rtc->tm.mday   = TOS_ReadRTCRegister(0x07);
    _rtc->tm.month  = TOS_ReadRTCRegister(0x08);
    _rtc->tm.year   = TOS_ReadRTCRegister(0x09);

    uint8_t bcd = TOS_ReadRTCRegister(0x0B);

    if (!(bcd & 0x04))
    {
        _rtc->tm.second  = (_rtc->tm.second & 0x0F) + (_rtc->tm.second / 16) * 10;
        _rtc->tm.minute  = (_rtc->tm.minute & 0x0F) + (_rtc->tm.minute / 16) * 10;
        _rtc->tm.hour    = ((_rtc->tm.hour  & 0x0F) + (_rtc->tm.hour   / 16) * 10) | (_rtc->tm.hour & 0x80);
        _rtc->tm.mday    = (_rtc->tm.mday   & 0x0F) + (_rtc->tm.mday   / 16) * 10;
        _rtc->tm.month   = (_rtc->tm.month  & 0x0F) + (_rtc->tm.month  / 16) * 10;
        _rtc->tm.year    = (_rtc->tm.year   & 0x0F) + (_rtc->tm.year   / 16) * 10;
        _rtc->tm.year    += 2000;
    }
}

void TOS_HandleInterruptRTC(TOS_IRQContext* context)
{
    if (_rtc != NULL)
    {
        _rtc->ticks++;
        _rtc->timer++;
        _rtc->seconds += RTC_MILLIS_TIME / 1000.0;

        if (_rtc->timer >= 512)
        {
            _rtc->timer = 0;
            TOS_UpdateRTC();
        }
    }    

    TOS_PortOutb(RTC_CMD, 0x0C);
    TOS_PortInb(RTC_DATA);
    TOS_AcknowledgeIRQ(context);
}

uint8_t TOS_ReadRTCRegister(uint8_t reg)
{
    TOS_PortOutb(RTC_CMD, reg);
    return TOS_PortInb(RTC_DATA);
}