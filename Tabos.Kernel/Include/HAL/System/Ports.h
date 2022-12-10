#pragma once
#include <Lib/Types.h>

/// @brief Read value from 8-bit I/O port @param port I/O port number @return Value from port
static inline uint8_t TOS_PortInb(uint16_t port) 
{ 
    uint8_t result;
    asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/// @brief Read value from 16-bit I/O port @param port I/O port number @return Value from port
static inline uint16_t TOS_PortInw(uint16_t port)
{
    uint16_t result;
    asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/// @brief Read value from 32-bit I/O port @param port I/O port number @return Value from port
static inline uint32_t TOS_PortInl(uint16_t port)
{
    uint32_t result;
    asm volatile("inl %%dx, %%eax" : "=a" (result) : "dN" (port));
    return result;
}

/// @brief Read string from I/O port @param port I/O port number @param data Pointer to output buffer @param sz Size in bytes
static inline void TOS_PortInsw(uint16_t port, uint8_t* data, size_t sz) { asm volatile("rep insw" : "+D" (data), "+c" (sz) : "d" (port) : "memory"); }

/// @brief Write value to 8-bit I/O port @param port I/O port number @param value Value to write
static inline void TOS_PortOutb(uint16_t port, uint8_t value) { asm volatile("out %%al, %%dx" : : "a" (value), "d" (port)); }

/// @brief Write value to 16-bit I/O port @param port I/O port number @param value Value to write
static inline void TOS_PortOutw(uint16_t port, uint16_t value) { asm volatile("out %%ax, %%dx" : : "a" (value), "d" (port)); }

/// @brief Write value to 32-bit I/O port @param port I/O port number @param value Value to write
static inline void TOS_PortOutl(uint16_t port, uint32_t value) { asm volatile("outl %%eax, %%dx" : : "a" (value), "dN" (port)); }

/// @brief Write string to I/O port @param port I/O port number @param data Pointer to input buffer @param sz Size in bytes
static inline void TOS_PortOutsw(uint16_t port, const uint8_t* data, size_t sz) { asm volatile("rep outsw" : "+S" (data), "+c" (sz) : "d" (port)); }