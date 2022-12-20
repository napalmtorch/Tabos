#include <HAL/DriverManager.h>
#include <Core/Common.h>

static TOS_PtrList _drivers;

bool drv_start(TOS_Driver* drv, void* arg)
{
    TOS_Log("STARTED TEST DRIVER\n");
    return true;
}

void drv_stop(TOS_Driver* drv)
{
    TOS_Log("STOPPED TEST DRIVER\n");
}

void TOS_InitDriverManager()
{
    _drivers = TOS_NewPtrList();

    TOS_InstallDriver((TOS_Driver*)TOS_GetDebugSerial());
    TOS_InstallAndStartDriver(TOS_NewDriver("PIT", 0xFFFFFF40, sizeof(TOS_PITTimer), (TOS_DriverStartEvent)TOS_StartPIT, (TOS_DriverStopEvent)TOS_StopPIT), (void*)1000);
    TOS_InstallAndStartDriver(TOS_NewDriver("RTC", 0xFFFFFF41, sizeof(TOS_RealTimeClock), (TOS_DriverStartEvent)TOS_StartRTC, (TOS_DriverStopEvent)TOS_StopRTC), NULL);

    TOS_Log("%s Initialized driver manager - %u drivers installed\n", DEBUG_OK, _drivers.count);
}

void TOS_InstallDriver(TOS_Driver* drv)
{
    if (drv == NULL) { TOS_Panic("TOS_InstallDriver(00000000) - Attempt to install null device driver"); return; }
    TOS_PtrListAdd(&_drivers, drv);
    TOS_Log("%s Installed device driver %p(%s) at %p\n", DEBUG_INFO, drv->id, drv->name, drv);
}

void TOS_InstallAndStartDriver(TOS_Driver* drv, void* arg)
{
    TOS_InstallDriver(drv);
    TOS_StartDriver(drv, arg);
}

void TOS_UninstallDriver(TOS_Driver* drv)
{
    if (drv == NULL) { TOS_Panic("TOS_UninstallDriver(00000000) - Attempt to uninstall null device driver"); return; }
    for (size_t i = 0; i < _drivers.count; i++)
    {
        TOS_Driver* d = TOS_PtrListAt(&_drivers, i);
        if (d == drv)
        {
            TOS_PtrListErase(&_drivers, i, false);
            TOS_Log("%s Uninstalled device driver %p(%s) at %p\n", DEBUG_INFO, drv->id, drv->name, drv);
            if (drv->is_threaded && drv != NULL) { TOS_TerminateThread(drv->thread); }
            TOS_FreeDriver(drv);
            return;
        }
    }
    TOS_Panic("TOS_UninstallDriver(%p) - Failed to uninstall device driver", drv);
}

void TOS_StartDriver(TOS_Driver* drv, void* arg)
{
    if (drv == NULL) { TOS_Panic("TOS_StartDriver(00000000) - Attempt to start null device driver"); return; }
    drv->start(drv, arg);
    if (drv->is_threaded && drv->thread != NULL) { TOS_StartThread(drv->thread); }
    TOS_Log("%s Started device driver %p(%s) at %p\n", DEBUG_INFO, drv->id, drv->name, drv);
}

void TOS_StopDriver(TOS_Driver* drv)
{
    if (drv == NULL) { TOS_Panic("TOS_StartDriver(00000000) - Attempt to start null device driver"); return; }
    drv->stop(drv);
    if (drv->is_threaded && drv->thread != NULL) { TOS_StopThread(drv->thread); }
    TOS_Log("%s Stopped device driver %p(%s) at %p\n", DEBUG_INFO, drv->id, drv->name, drv);
}

TOS_Driver* TOS_FetchDriver(uint32_t id)
{
    if (id == 0) { return NULL; }

    for (size_t i = 0; i < _drivers.count; i++)
    {
        TOS_Driver* drv = TOS_PtrListAt(&_drivers, i);
        if (drv->id == id) { return drv; }
    }
    return NULL;
}

TOS_Driver* TOS_FetchDriverByName(const char* name)
{
    if (name == NULL || TOS_StrLen(name) == 0) { return NULL; }

    for (size_t i = 0; i < _drivers.count; i++)
    {
        TOS_Driver* drv = TOS_PtrListAt(&_drivers, i);
        if (TOS_StrEquals(drv->name, name)) { return drv; }
    }
    return NULL;
}

TOS_Driver* TOS_FetchDriverAt(int index) { return TOS_PtrListAt(&_drivers, index); }

size_t TOS_GetDriverCount() { return _drivers.count; }