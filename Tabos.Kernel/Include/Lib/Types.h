#pragma once

#define attr_pack        __attribute__((packed))
#define attr_align(x)    __attribute__((aligned(x)))
#define attr_optimize(x) __attribute__((optimize(x)))

#define NULL 0

#define KILOBYTE (1024L)
#define MEGABYTE (KILOBYTE * 1024L)
#define GIGABYTE (MEGABYTE * 1024L)

#define INT8_MIN                (-128)
#define INT16_MIN               (-32767-1)
#define INT32_MIN               (-2147483647-1)
#define INT64_MIN               (-__INT64_C(9223372036854775807)-1)

#define INT8_MAX                (127)
#define INT16_MAX               (32767)
#define INT32_MAX               (2147483647)
#define INT64_MAX               (__INT64_C(9223372036854775807))

#define UINT8_MAX                (255)
#define UINT16_MAX               (65535)
#define UINT32_MAX               (4294967295U)
#define UINT64_MAX               (__UINT64_C(18446744073709551615))

#define LOW_16(x) ((uint16_t)(x))
#define HIGH_16(x) ((uint16_t)((x) >> 16))

typedef signed char   int8_t;
typedef short int     int16_t;
typedef int           int32_t;
typedef long long int int64_t;

typedef unsigned char          uint8_t;
typedef unsigned short int     uint16_t;
typedef unsigned int           uint32_t;
typedef unsigned long long int uint64_t;

typedef int                    intptr_t;
typedef unsigned int           uintptr_t;
typedef unsigned long int      size_t;

typedef char* va_list;
#define __va_argsiz(t) (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, pN) ((ap) = ((va_list) (&pN) + __va_argsiz(pN)))
#define va_end(ap)	((void)0)
#define va_arg(ap, t) (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))

typedef enum { false, true } bool;