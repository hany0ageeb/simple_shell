#ifndef IO_H_
#define IO_H_
int _fputs(const char *str, int fdout);
int _puts(const char *str);
int _fputc(char c, int fdout);
int _putc(char c);
int _fgetc(int fdin);
int _fprintf(int fdout, const char *format, ...);
int _printf(const char *format, ...);
#endif

