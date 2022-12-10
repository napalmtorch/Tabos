#include <Lib/Print.h>
#include <Core/Common.h>

#define PRINT_BUFFSZ 0x10000
#define PRINT_WORKSZ 0x1000

char _buffer[PRINT_BUFFSZ];
char _workbuff[PRINT_WORKSZ];

void TOS_FlushPrintBuffers()
{
    TOS_MemSet(_buffer,   0, PRINT_BUFFSZ);
    TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
}

void TOS_PrintArgs(const char* fmt, va_list args)
{
    TOS_FlushPrintBuffers();
    TOS_PrintArgsTo(_buffer, fmt, args);
    TOS_SerialPrint(TOS_GetDebugSerial(), _buffer);
}

void TOS_Print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TOS_PrintArgs(fmt, args);
    va_end(args);
}

void TOS_PrintArgsTo(char* buff, const char* fmt, va_list args)
{
    if (buff == NULL) { return; }
    if (fmt == NULL || TOS_StrLen(fmt) == 0) { return; }

    while (*fmt != 0)
    {
        if (*fmt == '%')
        {
            fmt++;
            if (*fmt == '%') { TOS_StrAppend(buff, '%'); fmt++; }
            else if (*fmt == 'c') { TOS_StrAppend(buff, va_arg(args, int)); }
            else if (*fmt == 'd')
            {
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertIntToStr(va_arg(args, int), _workbuff, 10));
            }
            else if (*fmt == 'u')
            {
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertUIntToStr(va_arg(args, uint32_t), _workbuff, 10));
            }
            else if (*fmt == 'x')
            {
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertUIntToStr(va_arg(args, uint32_t), _workbuff, 16));
            }
            else if (*fmt == '2')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { TOS_StrAppend(buff, *fmt); fmt++; continue; }
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertUIntToHex(va_arg(args, uint32_t), _workbuff, 1));
            }
            else if (*fmt == '4')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { TOS_StrAppend(buff, *fmt); fmt++; continue; }
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertUIntToHex(va_arg(args, uint32_t), _workbuff, 2));
            }
            else if (*fmt == '8')
            {
                fmt++;
                if (*fmt != 'x' && *fmt != 'X') { TOS_StrAppend(buff, *fmt); fmt++; continue; }
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertUIntToHex(va_arg(args, uint32_t), _workbuff, 4));
            }
            else if (*fmt == 'p')
            {
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertUIntToHex(va_arg(args, uint32_t), _workbuff, 4));
            }
            else if (*fmt == 'a')
            {
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);

                uint32_t v = va_arg(args, uint32_t);
                if (v < KILOBYTE)                       { TOS_StrConcat(buff, TOS_ConvertUIntToStr(v, _workbuff, 10)); TOS_StrConcat(buff, "B"); }
                else if (v >= KILOBYTE && v < MEGABYTE) { TOS_StrConcat(buff, TOS_ConvertUIntToStr(v / KILOBYTE, _workbuff, 10)); TOS_StrConcat(buff, "KB"); }
                else if (v >= MEGABYTE && v < GIGABYTE) { TOS_StrConcat(buff, TOS_ConvertUIntToStr(v / MEGABYTE, _workbuff, 10)); TOS_StrConcat(buff, "MB"); }
                else if (v >= GIGABYTE)                 { TOS_StrConcat(buff, TOS_ConvertUIntToStr(v / GIGABYTE, _workbuff, 10)); TOS_StrConcat(buff, "GB"); }
            }
            else if (*fmt == 'f')
            {
                TOS_MemSet(_workbuff, 0, PRINT_WORKSZ);
                TOS_StrConcat(buff, TOS_ConvertFloatToStr(va_arg(args, float), _workbuff, 3));
            }
            else if (*fmt == 's') { TOS_StrConcat(buff, va_arg(args, char*)); }
            else { TOS_StrAppend(buff, *fmt); }
        }
        else { TOS_StrAppend(buff, *fmt); }
        fmt++;
    }
}

void TOS_PrintTo(char* buff, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TOS_PrintArgsTo(buff, fmt, args);
    va_end(args);
}