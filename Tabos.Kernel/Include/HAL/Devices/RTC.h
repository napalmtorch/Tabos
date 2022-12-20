#pragma once
#include <Lib/Types.h>
#include <Lib/Time.h>
#include <HAL/Driver.h>
#include <HAL/Interrupts/IDT.h>

typedef struct
{
    TOS_Driver   driver;
    TOS_DateTime tm;
    uint32_t     ticks;
    uint32_t     tps, timer;
    float        seconds;
    uint32_t     millis, millis_total;
} TOS_RealTimeClock;

bool TOS_StartRTC(TOS_RealTimeClock* rtc, void* unused);
void TOS_StopRTC(TOS_RealTimeClock* rtc);
void TOS_HandleInterruptRTC(TOS_IRQContext* context);
uint8_t TOS_ReadRTCRegister(uint8_t reg);
