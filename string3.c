#include "string.h"
#include <stdlib.h>

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