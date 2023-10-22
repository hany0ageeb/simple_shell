#include "builtin.h"
#include "io.h"
#include "env.h"
#include "string.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/**
 * unsetenv_exec - execute unsetenv
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int unsetenv_exec(simple_command_t *command, sh_session_t *session)
{
	token_node_t *node = NULL;

	if (command->args == NULL || command->args->head == NULL)
		return (0);
	node = command->args->head;
	while (node != NULL)
	{
		_unsetenv(node->token->lexeme, &session->env_var_lst);
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

	if (command == NULL || command->cmd == NULL)
		return (-1);
	for (i = 0; session->env_var_lst[i] != NULL; ++i)
	{
		_puts(session->env_var_lst[i]);
		_putc('\n');
	}
	return (0);
}
/**
 * print_exit_error - print exit error
 * @session: session
 * @arg: argument
 * Return: void
 */
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
	_fputs(message, STDERR_FILENO);
	_fputc('\n', STDERR_FILENO);
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
		if ((exit_code == 0 && errno == EINVAL) || exit_code < 0)
		{
			print_exit_error(session, command->args->head);
			session->status = 2;
			session->exit_request = FALSE;
			return (2);
		}
		session->status = exit_code;
		session->exit_request = TRUE;
		return (0);
	}
	else
	{
		session->exit_request = TRUE;
		return (0);
	}
}
/**
 * _cd - cd into path
 * @path: path
 * @session: session
 * @pwd: $PWD
 * Return: 0 on success otherwise on fail.
 */
int _cd(const char *path, sh_session_t *session, const char *pwd)
{
	int ret = 0;
	char buffer[1024];

	ret = chdir(path);
	if (ret == 0)
	{
		ret = _setenv("OLDPWD", pwd, TRUE, &session->env_var_lst);
		if (ret == 0)
		{
			getcwd(buffer, 1024);
			ret = _setenv("PWD", buffer, TRUE, &session->env_var_lst);
		}
	}
	return (ret);
}
