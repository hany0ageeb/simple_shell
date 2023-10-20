#include "builtin.h"
#include "io.h"
#include "env.h"
#include "string.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
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
		_puts(": unsetenv: too few arguments\n");
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
		if (start_with(session->env_var_lst[i], "_=") == FALSE)
		{
			_puts(session->env_var_lst[i]);
			_putc('\n');
		}
	}
	return (0);
}
void print_exit_error(const sh_session_t *session, const token_node_t *arg)
{
	char *tmp;
	char *message;

	message = concat_str(session->sh_name, ": ");
	tmp = concat_str(message, ": exit: Illegal number: ");
	if (message != NULL)
	{
		free(message);
		message = NULL;
	}
	message = concat_str(tmp, arg->token->lexeme);
	_puts(message);
	_putc('\n');
	if (message != NULL)
		free(message);
	if (tmp != NULL)
		free(tmp);
}
/**
 * exit_exec - execute exit
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int exit_exec(simple_command_t *command, sh_session_t *session)
{
	int exit_code = session->status;

	if (command->args != NULL && command->args->head != NULL)
	{
		exit_code = str_to_int(command->args->head->token->lexeme);
		if (exit_code == 0 && errno == EINVAL)
		{
			print_exit_error(session, command->args->head);
			session->status = 2;
			return (2);
		}
		session->status = exit_code;
		exit(exit_code);
	}
	else
	{
		exit(session->status);
	}
}

