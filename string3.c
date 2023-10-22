#include "string.h"
#include <stdarg.h>
#include <stdlib.h>
/**
 * index_of - index of
 * @str: string
 * @lo: low index
 * @hi: high index
 * @c: char
 * Return: index of c first occurrence in str otherwise -1
 */
int index_of(const char *str, size_t lo, size_t hi, const char c)
{
	size_t i;

	for (i = lo; i <= hi && str[i] != '\0'; ++i)
	{
		if (str[i] == c)
			return ((ssize_t)i);
	}
	return (-1);
}
/**
 * _str - create a string of length len containg c repeated
 * @c: char
 * @len: length
 * Return: newly created string
 */
char *_str(const char c, size_t len)
{
	char *str = malloc(sizeof(char) * (len + 1));
	size_t i;

	if (str != NULL)
	{
		for (i = 0; i < len; ++i)
			str[i] = c;
		str[len] = '\0';
	}
	return (str);
}
/**
 * end_with_char - check if str end with c char
 * @str: string
 * @c: char
 * Return: TRUE if str last char is c otherwise FALSE
 */
bool_t end_with_char(const char *str, const char c)
{
	size_t len;

	if (IS_NULL_OR_EMPTY(str))
		return (FALSE);
	len = str_len(str);
	if (str[len - 1] == c)
		return (TRUE);
	return (FALSE);
}
/**
 * concat_strs - concat variadoc list of strings
 * @count: string count
 * Return: concated strings
 */
char *concat_strs(size_t count, ...)
{
	va_list argptr;
	size_t i, total_len = 0, j, k;
	char *tmp = NULL, *str = NULL;

	va_start(argptr, count);
	for (i = 0; i < count; ++i)
	{
		tmp = va_arg(argptr, char *);
		total_len += str_len(tmp);
	}
	va_end(argptr);
	if (total_len > 0)
	{
		str = malloc(sizeof(char) * (total_len + 1));
		if (str != NULL)
		{
			va_start(argptr, count);
			j = 0;
			for (i = 0; i < count; ++i)
			{
				tmp = va_arg(argptr, char *);
				if (!(IS_NULL_OR_EMPTY(tmp)))
				{
					for (k = 0; tmp[k] != '\0'; ++k, ++j)
						str[j] = tmp[k];
				}
			}
			str[total_len] = '\0';
			va_end(argptr);
		}
	}
	return (str);
}
