#include "io.h"
#include "mem.h"
#include <stdlib.h>
#include <errno.h>
ssize_t _getline(char **lineptr, size_t *n, int fdin)
{
    const size_t INITALLOC = 16;
    const size_t ALLOCSTEP = 16;
    size_t num_read = 0;
    size_t new_size;
    char *tmp;
    int c;

    if ((NULL == lineptr) || (NULL == n))
    {
        errno = EINVAL;
        return -1;
    }
    if (*lineptr == NULL)
    {
        *lineptr = malloc(sizeof(char) * (INITALLOC + 1));
        if (*lineptr == NULL)
            return (-1);
        else
            *n = INITALLOC;
    }
    while ((c = _fgetc(fdin)) != -1)
    {
        num_read++;
        if (num_read >= *n)
        {
            new_size = *n + ALLOCSTEP + 1;
            tmp = _realloc(*lineptr, *n, new_size);
            if (tmp == NULL)
                return (-1);
            else
                *lineptr = tmp;
        }
        (*lineptr)[num_read - 1] = (char)c;
        if (c == '\n')
        break;
    }
    (*lineptr)[num_read] = '\0';
    if (c == -1)
    {
        errno = 0;
        return (-1);
    }
    return ((ssize_t)num_read);
}
