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
char *concat_strs(size_t count, ...);
int str_cmp(const char *str1, const char *str2);
bool_t start_with(const char *str, const char *start);
size_t count_str_list_len(const char *str, const char separator);
char **split_str(const char *str, const char separator);
bool_t contains_char(const char *str, const char c);
bool_t str_equals(const char *str1, const char *str2);
int index_of(const char *str, size_t lo, size_t hi, const char c);
char *_str(const char c, size_t len);
bool_t end_with_char(const char *str, const char c);
#endif

