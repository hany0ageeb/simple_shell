#ifndef IO_H_
#define IO_H_
#define BUFF_SIZE 1024
#define FLUSH_BUFF 0
#include <stddef.h>
#include <sys/types.h>
#include "bool.h"
int _fputs(const char *str, int fdout);
int _puts(const char *str);
int _fputc(char c, int fdout);
int _putc(char c);
int _fgetc(int fdin);
/*int _fprintf(int fdout, const char *format, ...);*/
/*int _printf(const char *format, ...);*/
ssize_t _getline(char **lineptr, size_t *n, int fdin);
bool_t file_exists(const char *d, const char *f);
bool_t is_regular_file(const char *d, const char *f);
#endif

