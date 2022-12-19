#ifndef ATD_MEMORY_H
#define ATD_MEMORY_H 1

#ifdef TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdint.h>
    #include <stddef.h>
#endif

int ATD_memcmp(const void *left, const void *right, size_t sz);
int ATD_strcmp(const char *left, const char *right);
size_t ATD_strlen(const char *str);
void * ATD_malloc(size_t sz);
void   ATD_free  (void *data);

#endif