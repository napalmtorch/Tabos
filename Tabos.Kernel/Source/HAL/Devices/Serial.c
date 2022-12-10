#include <HAL/Devices/Serial.h>
#include <Core/Common.h>

static const char* _portnames[5] = { "disabled", "com1", "com2", "com3", "com4" };
static SERIAL_PORT _port         = SERIAL_DISABLED;

bool TOS_StartSerial(TOS_SerialController* serial, SERIAL_PORT port)
{
    // validate port
    if (!TOS_SerialValidatePort(port, true)) { return false; }
    if (port == SERIAL_DISABLED) { TOS_StopSerial(serial); return false; }

    // set serial port and send information to serial controller
    _port = port;
    TOS_PortOutb(_port + 1, 0x00);      // disable all interrupts
    TOS_PortOutb(_port + 3, 0x80);      // set baud rate divisor
    TOS_PortOutb(_port + 0, 0x03);      // set divisor to 3 (lo uint8_t) 38400 baud
    TOS_PortOutb(_port + 1, 0x00);      // hi uint8_t
    TOS_PortOutb(_port + 3, 0x03);      // 8 bits, no parity, one stop bit
    TOS_PortOutb(_port + 2, 0xC7);      // enable fifo, clear them, with 14-uint8_t threshold
    TOS_PortOutb(_port + 4, 0x0B);      // irqs enabled, rts/dsr set
    TOS_PortOutb(_port + 4, 0x1E);      // set in loopback mode, test the serial chip
    TOS_PortOutb(_port + 0, 0xAE);      // test serial chip (send uint8_t 0xAE and check if serial returns same uint8_t)

    // serial initialization failed - stop device
    if (TOS_PortInb(_port) != 0xAE) { TOS_StopSerial(serial); return false; }

    // stop testing chip
    TOS_PortOutb(_port + 4, 0x0F);
    return true;
}

void TOS_StopSerial(TOS_SerialController* serial)
{
    _port = SERIAL_DISABLED;
}

char TOS_SerialRead(TOS_SerialController* serial)
{
    if (!TOS_SerialValidatePort(_port, false)) { return 0; }
    while (!TOS_SerialCanRead(serial));
    return TOS_PortInb(_port);
}

void TOS_SerialWrite(TOS_SerialController* serial, char c)
{
    if (!TOS_SerialValidatePort(_port, false)) { return; }
    while (!TOS_SerialCanWrite(serial));
    TOS_PortOutb(_port, c);
}

void TOS_SerialPrint(TOS_SerialController* serial, const char* str)
{
    if (str == NULL || TOS_StrLen(str) == 0) { return; }

    register int i = 0;
    while (str[i] != 0) { TOS_SerialWrite(serial, str[i++]); }
}

bool TOS_SerialCanRead(TOS_SerialController* serial) { return TOS_PortInb(_port + 5) & 0x01; }

bool TOS_SerialCanWrite(TOS_SerialController* serial) { return TOS_PortInb(_port + 5) & 0x20; }

bool TOS_SerialValidatePort(SERIAL_PORT port, bool allow_disabled)
{
    switch (port)
    {
        case SERIAL_COM1: 
        case SERIAL_COM2: 
        case SERIAL_COM3: 
        case SERIAL_COM4:     { return true; }
        case SERIAL_DISABLED: { return allow_disabled; }
    }
    return false;
}

SERIAL_PORT TOS_GetSerialPort(TOS_SerialController* serial) { return _port; }

const char* TOS_SerialPortName(SERIAL_PORT port)
{
    switch (port)
    {
        default:          { return _portnames[0]; }
        case SERIAL_COM1: { return _portnames[1]; }
        case SERIAL_COM2: { return _portnames[2]; }
        case SERIAL_COM3: { return _portnames[3]; }
        case SERIAL_COM4: { return _portnames[4]; }
    }
}