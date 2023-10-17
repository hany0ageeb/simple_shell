#include "env.h"
#include <stddef.h>
#include <errno.h>
char *_getenv(char *name, char **envp)
{
    char *start_w = NULL;
    size_t i;

    if (envp == NULL)
        return (NULL);
    start_w = concat_str(name, "=");
    for (i = 0; envp[i] != NULL; ++i)
    {
        if (start_with(envp[i], start_w))
        {
            if (start_w != NULL)
                free(start_w);
            return (envp[i]);
        }
    }
    if (start_w != NULL)
        free(start_w);
    return (NULL);
}
int _setenv(const char *name, const char *value, bool_t overwrite, char **envp)
{
    int name_idx = -1;
    size_t i = 0;
    char *tmp;
    char *_name = NULL;

    if (IS_NULL_OR_EMPTY(name) == TRUE || contains_char(name, '=') == TRUE)
    {
        errno = EINVAL;
        return (-1);
    }
    while (envp[i] != NULL)
    {
        if (start_with(envp[i], name) == TRUE)
        {
            name_idx = i;
            break;
        }
        i++;
    }
    if (name_idx == -1)
    {
        _name = concat_str(name, "=");
        add_to_str_list(envp, _name);
        free(_name);
    }
    else if (name_idx >= 0 && overwrite == TRUE)
    {
        free(envp[i]);
        tmp = concat_str(name, "=");
        envp[i] = concat_str(tmp, value);
        if (tmp != NULL)
        {
            free(tmp);
            tmp = NULL;
        }
    }
    return (0);
}
int _unsetenv(const char *name, char **envp)
{
    char *start_with = NULL;

    if (IS_NULL_OR_EMPTY(name) || contains_char(name, '=') == TRUE)
    {
        errno = EINVAL;
        return (-1);
    }
    start_with = concat_str(name, "=");
    remove_from_str_list(envp, start_with);
    if (start_with != NULL)
    {
        free(start_with);
        start_with = NULL;
    }
    return (0);
}