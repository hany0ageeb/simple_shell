#ifndef BUILTIN_H_
#define BUILTIN_H_
#include "bool.h"
#include "simple_command.h"
#include "shell.h"
bool_t is_builtin_cmd(const char *lex);
int alias_exec(simple_command_t *command, sh_session_t *session);
int setenv_exec(simple_command_t *command, sh_session_t *session);
int unsetenv_exec(simple_command_t *command, sh_session_t *session);
int cd_exec(simple_command_t *command, sh_session_t *session);
int env_exec(simple_command_t *command, sh_session_t *session);
int exit_exec(simple_command_t *command, sh_session_t *session);

#endif

