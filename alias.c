#include "alias.h"
#include <stdlib.h>
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