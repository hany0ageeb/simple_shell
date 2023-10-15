#ifndef SIMPLE_COMMAND_H_
#define SIMPLE_COMMAND_H_
#include "token.h"
typedef struct simple_command
{
	struct simple_command *left;
	struct simple_command *right;
	token_t *cmd;
	token_t *op;
	token_list_t *args;
	bool_t is_builtin;
	int (*execute)(const struct simple_command *command, sh_session_t *session);

} simple_command_t;
simple_command_t *create_binary_command(simple_command_t *left, simple_command_t *right, token_t *op);
simple_command_t *create_simple_command(token_t *cmd, token_list_t *args);
void free_simple_command(simple_command_t **command);
char **get_args(const simple_command_t *command);
#endif

