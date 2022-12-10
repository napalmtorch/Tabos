#pragma once
#include <Lib/Types.h>
#include <Lib/PointerList.h>
#include <Core/Multitasking/Thread.h>

typedef struct TOS_Driver TOS_Driver;

typedef bool (*TOS_DriverStartEvent)(TOS_Driver* drv, void*);
typedef void (*TOS_DriverStopEvent)(TOS_Driver* drv);

struct TOS_Driver
{
    TOS_Thread*          thread;
    TOS_DriverStartEvent start;
    TOS_DriverStopEvent  stop;
    uint32_t            id;
    char*               name;
    bool                is_threaded;
};

TOS_Driver* TOS_NewDriver(const char* name, uint32_t id, size_t objsz, TOS_DriverStartEvent start, TOS_DriverStopEvent stop);
TOS_Driver* TOS_NewDriverThreaded(const char* name, uint32_t id, size_t objsz, size_t stacksz, TOS_PtrList args, TOS_ThreadProtocol protocol, TOS_DriverStartEvent start, TOS_DriverStopEvent stop);
void TOS_FreeDriver(TOS_Driver* drv);