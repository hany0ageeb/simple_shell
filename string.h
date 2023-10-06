#ifndef STRING_H_
#define STRING_H_
#include "bool.h"
#include <stddef.h>
#define IS_DIGIT(c) ((c >= '0' && c <= '9'))
#define IS_NULL(str) ((str == NULL))
#define IS_NULL_OR_EMPTY(str) ((IS_NULL(str) || *str == '\0'))
size_t str_len(const char *str);
char *copy_str(const char *str);
char *sub_str(const char *str, size_t lo, size_t hi);
char *concat_str(const char *str1, const char *str2);
#endif