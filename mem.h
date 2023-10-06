#ifndef MEM_H_
#define MEM_H_
#include <stddef.h>
void copy_mem(void *dest, const void *src, size_t size);
void *_realloc(void *ptr, size_t old_size, size_t new_size);
#endif
