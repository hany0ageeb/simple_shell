#include "string.h"
#include <stdlib.h>
#include <errno.h>

/**
 * str_len - mimic strlen function from standard library
 * @str: a pointer to null terminated string to calc its length
 * Return: 0 if str is NULL or empty otherwise
 * num of char in str except null terminated char '\0'
 */
size_t str_len(const char *str)
{
	size_t len = 0;

	if (IS_NULL_OR_EMPTY(str))
		return (0);
	while (str[len] != '\0')
		len++;
	return (len);
}
/**
 * copy_str - copy string pointed to by str1
 * @str: a string to copy
 * Return: exact copy of str1
 */
char *copy_str(const char *str)
{
	size_t len, i;
	char *cpy;

	if (IS_NULL(str))
		return (NULL);
	len = str_len(str);
	cpy = malloc(sizeof(char) * (len + 1));
	if (cpy != NULL)
	{
		for (i = 0; i < len ; ++i)
		{
			cpy[i] = str[i];
		}
		cpy[len] = '\0';
	}
	return (cpy);
}
/**
 * sub_str - sub string str from lo to hi inclusive.
 * @str: string
 * @lo: lower index 0 based
 * @hi: higher index 0 based
 * Return: str[lo - hi] inclusive or null if str is null or lo > hi
 */
char *sub_str(const char *str, size_t lo, size_t hi)
{
	char *tmp = NULL;
	size_t len = 0, i = 0, len_str;

	if (lo > hi || IS_NULL_OR_EMPTY(str))
		return (NULL);
	len_str = str_len(str);
	hi = hi >= len_str ? len_str : hi;
	lo = lo >= len_str ? len_str : lo;
	len = hi - lo + 1;
	tmp = malloc(sizeof(char) * (len + 1));
	if (tmp != NULL)
	{
		while (lo <= hi && i < len && lo < len_str)
		{
			tmp[i] = str[lo];
			lo++;
			i++;
		}
		tmp[len] = '\0';
		errno = 0;
	}
	else
	{
		errno = ENOMEM;
	}
	return (tmp);
}
/**
 * concat_str - concate strings pointed to by str1 followed by str2
 * @str1: first string
 * @str2: second string
 * Return: str1 + str2
 */
char *concat_str(const char *str1, const char *str2)
{
	size_t len1, len2, len, i, j;
	char *tmp = NULL;

	if (IS_NULL(str1) && IS_NULL(str2))
		return (NULL);
	len1 = str_len(str1);
	len2 = str_len(str2);
	len = len1 + len2;
	tmp = malloc(sizeof(char) * (len + 1));
	if (tmp != NULL)
	{
		for (i = 0, j = 0; i < len && j < len1; ++i, ++j)
			tmp[i] = str1[j];
		for (j = 0; i < len && j < len2 ; ++i, ++j)
			tmp[i] = str2[j];
		tmp[len] = '\0';
	}
	return (tmp);
}
/**
 * str_cmp - compares the string pointed to,
 * by str1 to the string pointed to by str2.
 * @str1: first string
 * @str2: second string
 * Return: if str1 < str2 return negative value
 * if str1 equals str2 return 0
 * if str1 > str2 return positive value
 */
int str_cmp(const char *str1, const char *str2)
{
	size_t i;

	for (i = 0; str1[i] != '\0' && str2[i] != '\0'; ++i)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
	}
	if (str1[i] != '\0')
		return (str1[i] - 0);
	else if (str2[i] != '\0')
		return (0 - str2[i]);
	else
		return (0);
}

