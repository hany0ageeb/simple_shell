#include "alias.h"
#include <stdlib.h>
/**
 * create_alias - create alias object
 * @name: alias name
 * @value: alias value
 * Return: a pointer to newly created alias object
 */
alias_t *create_alias(char *name, char *value)
{
	alias_t *a = malloc(sizeof(alias_t));

	if (a != NULL)
	{
		a->name = name;
		a->value = value;
	}
	return (a);
}
/**
 * free_alias - free alias object memory
 * @a: alias object
 * Return: void
 */
void free_alias(alias_t **a)
{
	if (a != NULL && *a != NULL)
	{
		if ((*a)->name != NULL)
		{
			free((*a)->name);
			(*a)->name = NULL;
		}
		if ((*a)->value != NULL)
		{
			free((*a)->value);
			(*a)->value = NULL;
		}
		free(*a);
		*a = NULL;
	}
}

