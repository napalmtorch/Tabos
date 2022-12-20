#include <Lib/Time.h>
#include <Core/Common.h>

TOS_DateTime TOS_NewDateTime(int sec, int min, int hr, int mday, int mn, int yr, int wday, int yday)
{
    TOS_DateTime time = (TOS_DateTime)
    {
        sec, min, hr, mday, mn, yr, wday, yday,
    };
    return time;
}

TOS_DateTime TOS_GetTimeNow(TOS_DateTime* tm)
{
    TOS_RealTimeClock* rtc = (TOS_RealTimeClock*)TOS_FetchDriverByName("RTC");
    if (rtc == NULL) { return TOS_NewDateTime(0, 0, 0, 0, 0, 0, 0, 0); }

    TOS_DateTime time = rtc->tm;
    if (tm != NULL) { *tm = time; }

    return time;
}

uint32_t TOS_GetMillisecondsNow(uint32_t* millis)
{
    TOS_RealTimeClock* rtc = (TOS_RealTimeClock*)TOS_FetchDriverByName("RTC");
    if (rtc == NULL) { return 0; }

    uint32_t time = rtc->millis_total;
    if (millis != NULL) { *millis = time; }

    return time;
}

float TOS_GetSecondsNow(float* secs)
{
    TOS_RealTimeClock* rtc = (TOS_RealTimeClock*)TOS_FetchDriverByName("RTC");
    if (rtc == NULL) { return 0; }

    float time = rtc->seconds;
    if (secs != NULL) { *secs = time; }

    return time;
}