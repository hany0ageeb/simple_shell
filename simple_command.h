#ifndef SIMPLE_COMMAND_H_
#define SIMPLE_COMMAND_H_
#include "token.h"
#include "bool.h"
#include "session.h"
/**
 * struct simple_command - simple command
 * @left: left command
 * @right: right command
 * @op: binary operator
 * @cmd: command token
 * @args: command list args tokens
 * @is_builtin: flag
 * @execute: execute function pointer
 */
typedef struct simple_command
{
	struct simple_command *left;
	struct simple_command *right;
	token_t *cmd;
	token_t *op;
	token_list_t *args;
	bool_t is_builtin;
	int (*execute)(struct simple_command *command, struct sh_session *session);

} simple_command_t;
simple_command_t *create_binary_command(simple_command_t *left,
		simple_command_t *right, token_t *op);
simple_command_t *create_simple_command(token_t *cmd, token_list_t *args);
void free_simple_command(simple_command_t **command);
char **get_args(const simple_command_t *command);
#endif

