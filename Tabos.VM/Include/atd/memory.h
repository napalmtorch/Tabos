#ifndef ATD_MEMORY_H
#define ATD_MEMORY_H 1
#include <stdint.h>
#include <stddef.h>

int ATD_memcmp(const void *left, const void *right, size_t sz);
int ATD_strcmp(const char *left, const char *right);
size_t ATD_strlen(const char *str);

#endif