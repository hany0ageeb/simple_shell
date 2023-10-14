#include "io.h"
#include "string.h"
#include <unistd.h>
/**
 * _puts - mimic puts function
 * @str: string to write to std output
 * Return: -1 if error or the number of chars written
 */
int _puts(const char *str)
{
	return (_fputs(str, STDOUT_FILENO));
}
/**
 * _fputs - mimic fputs function
 * @str: a string to write
 * @fdout: file descriptor to write to
 * Return: -1 if error or the number of char written
 */
int _fputs(const char *str, int fdout)
{
	size_t len;

	if (IS_NULL(str))
		return (-1);
	len = str_len(str);
	return (write(fdout, str, len));
}
/**
 * _putc - mimic putc function
 * send c to std output
 * @c: char to write
 * Return: -1 if error or 1 if success
 */
int _putc(char c)
{
	return (_fputc(c, STDOUT_FILENO));
}
/**
 * _fputc - write a char to fdout
 * @c: char to write
 * @fdout: file descriptor
 * Return: -1 if error or 1 if success
 */
int _fputc(char c, int fdout)
{
	static char buffer[BUFF_SIZE + 1];
	static size_t index = -1;

	if (c == FLUSH_BUFF || index >= BUFF_SIZE)
	{
		if (index > -1)
		{
			buffer[index + 1] = '\0';
			index = -1;
			return (_fputs(buffer, fdout));
		}
		return (0);
	}
	else
	{
		buffer[++index] = c;
		return (1);
	}
}
/**
 * _fgetc - mimic fgetc function
 * @fdin: file descriptor to read from
 * Return: -1 if error or 1 if success
 */
int _fgetc(int fdin)
{
	char c;
	ssize_t n_read;

	n_read = read(fdin, &c, 1);
	if (n_read == 1)
		return (c);
	return (-1);
}

