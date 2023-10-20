#ifndef STR_LIST_H_
#define STR_LIST_H_
#include <stddef.h>
char **create_str_list(size_t len, ...);
size_t str_list_len(char **str_list);
void add_to_str_list(char ***str_list, const char *value);
void remove_from_str_list(char **str_list, const char *start);
void free_str_list(char **str_list);
#endif

