#include "mem.h"
#include <stdlib.h>

/**
 * _realloc - mimic realloc function
 * @ptr: pointer
 * @old_size: old size
 * @new_size: new size
 * Return: pointer
 */
void *_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *tmp;

	if (ptr == NULL)
	{
		if (new_size > 0)
			tmp = malloc(new_size);
		else
			tmp = NULL;
		return (tmp);
	}
	else
	{
		if (new_size == 0)
		{
			free(ptr);
			return (NULL);
		}
		else
		{
			tmp = malloc(new_size);
			copy_mem(tmp, ptr, old_size < new_size ? old_size : new_size);
			free(ptr);
			return (tmp);
		}
	}
}
/**
 * copy_mem - copy memory
 * @dest: destination
 * @src: source
 * @size: size
 * Return: void
 */
void copy_mem(void *dest, const void *src, size_t size)
{
	char *src_str, *dest_str;
	size_t i;

	if (dest != NULL && src != NULL && size > 0)
	{
		src_str = (char *)src;
		dest_str = (char *)dest;
		for (i = 0; i < size; ++i)
			dest_str[i] = src_str[i];
	}
}

