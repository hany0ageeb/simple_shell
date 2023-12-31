#include "string.h"
#include <stdlib.h>
#include <stdio.h>
/**
 * start_with - check if a string start with another string
 * @str: string to check
 * @start: start with string
 * Return: True if str start with start otherwise FALSE
*/
bool_t start_with(const char *str, const char *start)
{
	size_t i;

	if (str == NULL || start == NULL)
		return (FALSE);
	for (i = 0; str[i] != '\0' && start[i] != '\0'; ++i)
	{
		if (str[i] != start[i])
			return (FALSE);
	}
	if (start[i] != '\0')
		return (FALSE);
	if (str[i] == '\0' && start[i] != '\0')
		return (FALSE);
	return (TRUE);
}
/**
 * count_str_list_len  - count string list len
 * @str: to be splited string
 * @separator: separartor
 * Return: length
 */
size_t count_str_list_len(const char *str, const char separator)
{
	size_t len = 0, i;

	if (IS_NULL(str))
		return (len);
	len = 1;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (str[i] == separator)
			len++;
	}
	return (len);
}
/**
 * split_str - split a string by separator char
 * @str: string to split
 * @separator: separartor char
 * Return: null terminated list of strings
*/
char **split_str(const char *str, const char separator)
{
	size_t start = 0, current = 0, len, i;
	char **str_list = NULL;

	if (IS_NULL(str))
		return (NULL);
	len = count_str_list_len(str, separator);
	str_list = malloc(sizeof(char *) * (len + 1));
	str_list[len] = NULL;
	i = 0;
	while (str[current] != '\0' && i < len)
	{
		if (str[current] == separator)
		{
			if (current == start)
				str_list[i] = copy_str("");
			else
				str_list[i] = sub_str(str, start, current - 1);
			i++;
			current++;
			start = current;
		}
		else
			current++;
	}
	if (i < len)
	{
		if (current == start)
			str_list[i] = copy_str("");
		else
			str_list[i] = sub_str(str, start, current - 1);
	}
	return (str_list);
}
/**
 * contains_char - check if str contains c
 * @str: string to check
 * @c: the char t check its existence in str
 * Return: TRUE if c in str otherwise FALSE
*/
bool_t contains_char(const char *str, const char c)
{
	size_t i;

	if (IS_NULL_OR_EMPTY(str))
		return (FALSE);
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (c == str[i])
			return (TRUE);
	}
	return (FALSE);
}
/**
 * str_equals - check if two strings are equal
 * @str1: first string
 * @str2: second string
 * Return: TRUE if both ar equal otherwise FALSE
 */
bool_t str_equals(const char *str1, const char *str2)
{
	size_t i;

	if (str1 == str2)
		return (TRUE);
	if (str1 == NULL || str2 == NULL)
		return (FALSE);
	for (i = 0; str1[i] != '\0' && str2[i] != '\0'; ++i)
	{
		if (str1[i] != str2[i])
			return (FALSE);
	}
	if (str1[i] != str2[i])
		return (FALSE);
	return (TRUE);
}

