#include <Lib/Convert.h>
#include <Core/Common.h>

void _itoa_swap(char *x, char *y) { char t = *x; *x = *y; *y = t; }

char* _itoa_rev(char *buffer, int i, int j)
{
    while (i < j) { _itoa_swap(&buffer[i++], &buffer[j--]); }
    return buffer;
}

void _ultoa(unsigned long value, char* result, int base)
{
    unsigned char index;
    char buffer[32];
    index = 32;
    do 
    {
        buffer[--index] = '0' + (value % base);
        if ( buffer[index] > '9') { buffer[index] += 'A' - ':'; }
        value /= base;
    } while (value != 0);

    do { *result++ = buffer[index++]; } while (index < 32);
    *result = 0;
}

void _ftoa_rev(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int _ftoa_conv(int x, char str[], int d)
{
    int i = 0;
    while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }

    while (i < d) { str[i++] = '0'; }

    _ftoa_rev(str, i);
    str[i] = '\0';
    return i;
}


int TOS_ConvertStrToInt(const char* str) { return (int)TOS_ConvertStrToUInt(str); }

uint32_t TOS_ConvertStrToUInt(const char* str)
{
    int result = 0, sign = 0;
    uint32_t digit;
    while (*str == 0x20) { str += 1; }
    
    if (*str == '-') { sign = 1; str += 1; } 
    else { sign = 0; if (*str == '+') { str += 1; } }

    for (;; str += 1) 
    {
        digit = *str - '0';
        if (digit > 9) { break; }
        result = (10 * result) + digit;
    }
    if (sign) { return -result; }
    return result;
}

uint32_t TOS_ConvertHexToUInt(const char* str)
{
    uint32_t num = 0;
    for (size_t i = 0; i < TOS_StrLen(str); i++)
    {
        if (str[i] >= 48 && str[i] <= 57) { num = (num << 4) | (str[i] - 48); }
        else if (str[i] == 'A' || str[i] == 'a') { num = (num << 4) | 0x0A; }
        else if (str[i] == 'B' || str[i] == 'b') { num = (num << 4) | 0x0B; }
        else if (str[i] == 'C' || str[i] == 'c') { num = (num << 4) | 0x0C; }
        else if (str[i] == 'D' || str[i] == 'd') { num = (num << 4) | 0x0D; }
        else if (str[i] == 'E' || str[i] == 'e') { num = (num << 4) | 0x0E; }
        else if (str[i] == 'F' || str[i] == 'f') { num = (num << 4) | 0x0F; }
        else { return 0 ; }
    }
    return num;
}

float TOS_ConvertStrToFloat(const char* str)
{
    /* TODO: Not yet implemented */
    return 0.0f;
}

char* TOS_ConvertIntToStr(int num, char* buff, int base)
{
    if (base < 2 || base > 32) { return buff; }

    int i = 0, n = TOS_Abs(num); 
    while (n)
    {
        int r = n % base;
        if (r >= 10) { buff[i++] = 65 + (r - 10); }
        else { buff[i++] = 48 + r; }
        n = n / base;
    }

    if (i == 0) { buff[i++] = '0'; }
    if (num < 0 && base == 10) { buff[i++] = '-'; }
    buff[i] = '\0';
    return _itoa_rev(buff, 0, i - 1);
}

char* TOS_ConvertUIntToStr(uint32_t num, char* buff, int base)
{
    _ultoa(num, buff, base); 
    return buff;
}

char* TOS_ConvertFloatToStr(float num, char* buff, int dot)
{
    int ipart = (int)num;
    float fpart = num - (float)ipart;
    int i = _ftoa_conv(ipart, buff, 0);

    if (dot != 0) 
    {
        buff[i] = '.';
        fpart = fpart * TOS_PowF(10, dot);
        _ftoa_conv((int)fpart, buff + i + 1, dot);
    }
    return buff;
}

char* TOS_ConvertUIntToHex(uint32_t num, char* buff, uint8_t sz)
{
    static const char hexvals[] = "0123456789ABCDEF";

    char* start = buff;
    start[0] = 0;

    if (sz == 1)
    {
        *buff = hexvals[(uint8_t)((num & 0xF0) >> 4)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0F))];
    }
    else if (sz == 2)
    {
        *buff = hexvals[(uint8_t)((num & 0xF000) >> 12)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0F00) >> 8)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x00F0) >> 4)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x000F))]; 
    }
    else if (sz == 4)
    {
        *buff = hexvals[(uint8_t)((num & 0xF0000000) >> 28)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0F000000) >> 24)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x00F00000) >> 20)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x000F0000) >> 16)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x0000F000) >> 12)]; buff++;
        *buff = hexvals[(uint8_t)((num & 0x00000F00) >> 8)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x000000F0) >> 4)];  buff++;
        *buff = hexvals[(uint8_t)((num & 0x0000000F))];
    }
    return start;
}
