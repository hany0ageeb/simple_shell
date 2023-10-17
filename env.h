#ifndef ENV_H_
#define ENV_H_
#include "bool.h"
char *_getenv(char *name, char **envp);
int _setenv(const char *name, const char *value, bool_t overwrite, char **envp);
int _unsetenv(const char *name, char **envp);
#endif