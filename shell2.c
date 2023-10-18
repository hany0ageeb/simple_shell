#include "shell.h"
#include "string.h"

/**
 * is_valid_num - is valid number string
 * @str: string
 * Return: TRUE if str [+-]*[0-9]{1...} otherwise FALSE
 */
bool_t is_valid_num(const char *str)
{
	int i;

	if (IS_NULL_OR_EMPTY(str))
		return (FALSE);
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	else
		i = 0;
	for (; str[i] != '\0'; ++i)
	{
		if (!(IS_DIGIT(str[i])))
		{
			return (FALSE);
		}
	}
	return (TRUE);
}
/**
 * int_len - int length
 * @num: number
 * Return: length
 */
int int_len(ssize_t num)
{
	int len = 0;

	if (num < 0)
	{
		len++;
		num *= -1;
	}
	do {
		len++;
		num /= 10;
	} while (num > 0);
	return (len);
}
/**
 * int_to_str - int to string
 * @num: number
 * Return: num as string
 */
char *int_to_str(ssize_t num)
{
	const char *digits = "0123456789";
	int len = int_len(num);
	int i = len - 1;
	char *str = malloc(sizeof(char) * (len + 1));

	if (str != NULL)
	{
		if (num < 0)
		{
			str[0] = '-';
			num *= -1;
		}
		do {
			str[i] = digits[num % 10];
			num /= 10;
			i--;
		} while (num > 0);
		str[len] = '\0';
	}
	return (str);
}
/**
 * str_to_int - string to integer
 * @str: string
 * Return: str as int
 */
int str_to_int(const char *str)
{
	const int BASE = 10;
	int value = 0, w = 1, i;
	size_t len;

	if (is_valid_num(str))
	{
		len = str_len(str);
		for (i = len - 1; i >= 0; --i)
		{
			if (i == 0 && i == '-')
				value *= -1;
			else
			{
				value += (str[i] - 48) * w;
				w *= BASE;
			}
		}
		errno = 0;
		return (value);
	}
	else
	{
		errno = EINVAL;
		return (0);
	}
}

