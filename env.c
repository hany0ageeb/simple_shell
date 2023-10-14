#include "string.h"
#include "shell.h"
char *_getenv(const char *name, const char **envp)
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
char **get_paths(const char **envp)
{
    if (envp == NULL)
        return (NULL);
    char *path = _getenv("PATH", envp);
    if (path != NULL)
    {
        return (split_str(path, ':'));
    }
    return (NULL);
}