#include <Core/Debug.h>
#include <Core/Common.h>

static TOS_SerialController _serial;

void TOS_InitDebug()
{
    _serial.driver.id          = 0xFFFFFF00;
    _serial.driver.start       = (TOS_DriverStartEvent)TOS_StartSerial;
    _serial.driver.stop        = (TOS_DriverStopEvent)TOS_StopSerial;
    _serial.driver.is_threaded = false;
    _serial.driver.thread      = NULL;
    _serial.driver.name        = "Serial";
    TOS_StartSerial(&_serial, SERIAL_COM1);
}

void TOS_Log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TOS_PrintArgs(fmt, args);
    va_end(args);
}

void TOS_Error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TOS_Print("%s ", DEBUG_ERROR);
    TOS_PrintArgs(fmt, args);
    TOS_Print("\n");
    va_end(args);
}

void TOS_Panic(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TOS_Print("%s ", DEBUG_ERROR);
    TOS_PrintArgs(fmt, args);
    va_end(args);
    TOS_Log("\nThe system has been been halted to prevent any potential damage to\nyour computer.\n");
    asm volatile("cli; hlt");
}

void TOS_Breakpoint()
{
    asm volatile("xchg %bx, %bx");
}

TOS_SerialController* TOS_GetDebugSerial()
{
    return (TOS_SerialController*)&_serial;
}