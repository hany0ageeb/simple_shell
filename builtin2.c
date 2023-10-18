#include "builtin.h"
#include "io.h"
#include "env.h"

/**
 * unsetenv_exec - execute unsetenv
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int unsetenv_exec(simple_command_t *command, sh_session_t *session)
{
	char *lin;
	token_node_t *node = NULL;

	if (command->args == NULL || command->args->head == NULL)
	{
		_puts(session->sh_name);
		_puts(": ");
		lin = int_to_str(command->cmd->line);
		_puts(lin);
		_puts(": unsetenv: too few arguments");
		_putc('\n');
		return (1);
	}
	node = command->args->head;
	while (node != NULL)
	{
		_unsetenv(node->token->lexeme, session->env_var_lst);
		node = node->next;
	}
	return (0);
}
/**
 * env_exec - execute env
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int env_exec(simple_command_t *command, sh_session_t *session)
{
	size_t i;

	if (command == NULL || command->cmd == NULL
			|| !str_equals(command->cmd->lexeme, "env"))
		return (-1);
	for (i = 0; session->env_var_lst[i] != NULL; ++i)
	{
		_puts(session->env_var_lst[i]);
		_putc('\n');
	}
	return (0);
}
/**
 * exit_exec - execute exit
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int exit_exec(simple_command_t *command, sh_session_t *session)
{
	int exit_code = 0;
	char *message = NULL;
	char *tmp = NULL;

	if (command->args != NULL && command->args->head != NULL)
	{
		exit_code = str_to_int(command->args->head->token->lexeme);
		if (exit_code == 0 && errno == EINVAL)
		{
			tmp = int_to_str(command->cmd->line);
			message = concat_str(session->sh_name, tmp);
			if (tmp != NULL)
			{
				free(tmp);
				tmp = NULL;
			}
			_puts(message);
			if (message != NULL)
			{
				free(message);
				nessage = NULL;
			}
			session->status = 2;
			return (2);
		}
	}
	session->status = exit_code;
	exit(exit_code);
}

