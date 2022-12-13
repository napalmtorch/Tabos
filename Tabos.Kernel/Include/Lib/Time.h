#pragma once
#include <Lib/Types.h>

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t mday;
    uint8_t month;
    uint8_t year;
    uint8_t wday;
    uint8_t yday;
} TOS_DateTime;

static inline TOS_DateTime TOS_NewDateTime(int sec, int min, int hr, int mday, int mn, int yr, int wday, int yday)
{
    TOS_DateTime time = (TOS_DateTime)
    {
        sec, min, hr, mday, mn, yr, wday, yday,
    };
    return time;
}