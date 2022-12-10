#pragma once
#include <Lib/Types.h>
#include <Lib/PointerList.h>
#include <HAL/Driver.h>

void TOS_InitDriverManager();
void TOS_InstallDriver(TOS_Driver* drv);
void TOS_InstallAndStartDriver(TOS_Driver* drv, void* arg);
void TOS_UninstallDriver(TOS_Driver* drv);
void TOS_StartDriver(TOS_Driver* drv, void* arg);
void TOS_StopDriver(TOS_Driver* drv);

TOS_Driver* TOS_FetchDriver(uint32_t id);
TOS_Driver* TOS_FetchDriverFromName(const char* name);
TOS_Driver* TOS_FetchDriverAt(int index);

size_t TOS_GetDriverCount();