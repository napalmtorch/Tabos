#pragma once
#include <Lib/Types.h>
#include <HAL/Driver.h>
#include <HAL/Interrupts/IDT.h>

typedef struct
{
    TOS_Driver driver;
    uint32_t   ticks;
    uint32_t   tps, timer, freq;
    uint32_t   millis, millis_total;
} TOS_PITTimer;

bool TOS_StartPIT(TOS_PITTimer* pit, uint32_t freq);
void TOS_StopPIT(TOS_PITTimer* pit);
void TOS_HandleInterruptPIT(TOS_IRQContext* context);
