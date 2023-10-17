#include "str_list.h"
#include "string.h"
#include <stdlib.h>
#include <stdarg.h>

size_t str_list_len(char **str_list)
{
    size_t len = 0;
    if (str_list == NULL)
        return (len);
    while (str_list[len] != NULL)
        len++;
    return (len);
}
void add_to_str_list(char **str_list, const char *value)
{
    size_t len, i = 0;
    char **tmp = NULL;
    if (IS_NULL_OR_EMPTY(value))
    return;
    len = str_list_len(str_list);
    tmp = malloc(sizeof(char *) * (len + 2));
    while (str_list[i] != NULL)
    {
        tmp[i] = copy_str(str_list[i]);
        i++;
    }
    tmp[i] = copy_str(value);
    tmp[i + 1] = NULL;
    free_str_list(str_list);
    str_list = tmp;
}
void remove_from_str_list(char **str_list, const char *start)
{
    size_t i = 0, len, j = 0;
    int exist_index = -1;
    char **tmp;

    if (IS_NULL_OR_EMPTY(start))
    return;
    for (i = 0; str_list[i] != NULL; ++i)
    {
        if (start_with(str_list[i], start))
        {
            exist_index = i;
            break;
        }
    }
    if (exist_index >= 0)
    {
        len = str_list_len(str_list);
        tmp = malloc(sizeof(char *) * (len));
        while (str_list[i] != NULL)
        {
            if (((int)i) != exist_index)
            {
                tmp[j] = str_list[i];
                str_list[i] = NULL;
            }
            else
            {
                free(str_list[i]);
                str_list[i] = NULL;
            }
            i++;
        }
        free(str_list);
        str_list = tmp;
    }
}
char **create_str_list(size_t len, ...)
{
	char **lst = malloc(sizeof(char *) * (len + 1));
	va_list argptr;
	size_t i;

	if (lst != NULL)
	{
		va_start(argptr, len);
		for (i = 0; i < len; ++i)
		{
			lst[i] = copy_str(va_arg(argptr, char *));
		}
		va_end(argptr);
		lst[len] = NULL;
	}
	return (lst);
}
void free_str_list(char **str_list)
{
	size_t i;

	if (str_list != NULL)
	{
		for (i = 0; str_list[i] != NULL; ++i)
		{
			free(str_list[i]);
			str_list[i] = NULL;
		}
		free(str_list);
		str_list = NULL;
	}
}

