#include "io.h"
#include "string.h"
#include <unistd.h>
int _puts(const char *str)
{
    return (_fputs(str, STDOUT_FILENO));
}
int _fputs(const char *str, int fdout)
{
    size_t len;

    if (IS_NULL(str))
    return (-1);
    len = str_len(str);
    return (write(fdout, str, len));
}
int _putc(char c)
{
    return (_fputc(c, STDOUT_FILENO));
}
int _fputc(char c, int fdout)
{
    return (write(fdout, &c, 1));
}
int _fgetc(int fdin)
{
    char c;
    ssize_t n_read = read(fdin, &c, 1);

    if (n_read == -1)
    return (-1);
    return ((int)c);
}