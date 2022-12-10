#pragma once
#include <Lib/Types.h>
#include <HAL/System/Ports.h>

static inline void TOS_RemapPIC()
{
    TOS_PortOutb(0x20, 0x11);
    TOS_PortOutb(0xA0, 0x11);
    TOS_PortOutb(0x21, 0x20);
    TOS_PortOutb(0xA1, 0x28);
    TOS_PortOutb(0x21, 0x04);
    TOS_PortOutb(0xA1, 0x02);
    TOS_PortOutb(0x21, 0x01);
    TOS_PortOutb(0xA1, 0x01);
    TOS_PortOutb(0x21, 0x0);
    TOS_PortOutb(0xA1, 0x0);
}