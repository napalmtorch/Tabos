#include <HAL/Driver.h>
#include <Core/Common.h>

TOS_Driver* TOS_NewDriver(const char* name, uint32_t id, size_t objsz, TOS_DriverStartEvent start, TOS_DriverStopEvent stop)
{
    TOS_Driver* driver   = TOS_Alloc(objsz < sizeof(TOS_Driver) ? sizeof(TOS_Driver) : objsz);
    driver->thread      = NULL;
    driver->is_threaded = false;
    driver->name        = TOS_NewString(name);
    driver->id          = id;
    driver->start       = start;
    driver->stop        = stop;
    TOS_Log("%s Created driver - ID:%p Size:%d Start:%p Stop:%p Name:%s\n", DEBUG_OK, id, objsz, start, stop, name);
    return driver;
}

TOS_Driver* TOS_NewDriverThreaded(const char* name, uint32_t id, size_t objsz, size_t stacksz, TOS_PtrList args, TOS_ThreadProtocol protocol, TOS_DriverStartEvent start, TOS_DriverStopEvent stop)
{
    TOS_Driver* driver   = TOS_Alloc(objsz < sizeof(TOS_Driver) ? sizeof(TOS_Driver) : objsz);
    driver->thread      = TOS_NewThread(name, stacksz, protocol, THREAD_PRIORITY_NORMAL, args);
    driver->is_threaded = true;
    driver->name        = driver->thread->name;
    driver->id          = id;
    driver->start       = start;
    driver->stop        = stop;
    TOS_Log("%s Created driver - ID:%p Size:%d Start:%p Stop:%p Main:%p Name:%s\n", DEBUG_OK, id, objsz, start, stop, protocol, name);
    return driver;
}

void TOS_FreeDriver(TOS_Driver* drv)
{
    if (drv == NULL) { TOS_Panic("TOS_FreeDriver(00000000) - Attempt to free null driver"); return; }
    if (!drv->is_threaded) { TOS_Free(drv->name); }
    TOS_Free(drv);
}