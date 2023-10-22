#include "builtin.h"
#include "io.h"
#include "string.h"
#include "alias_list.h"
#include "env.h"
#include "shell.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/**
 * print_cd_error - print cd error
 * @sh: program name
 * @line: line
 * @dir: dir name
 * Return: void
 */
static void print_cd_error(const char *sh, size_t line, const char *dir)
{
	char *lin = int_to_str(line);

	_fputs(sh, STDERR_FILENO);
	_fputs(": ", STDERR_FILENO);
	_fputs(lin, STDERR_FILENO);
	_fputs(": cd: can't cd to ", STDERR_FILENO);
	_fputs(dir, STDERR_FILENO);
	_fputc('\n', STDERR_FILENO);
	if (lin != NULL)
	{
		free(lin);
		lin = NULL;
	}
}
/**
 * is_builtin_cmd - check to see if lex is a builtin command
 * @lex: command name
 * Return: True if builtin command otherwise false
*/
bool_t is_builtin_cmd(const char *lex)
{
	if (lex == NULL)
		return (FALSE);
	if (str_equals(lex, "exit") || str_equals(lex, "env"))
		return (TRUE);
	else if (str_equals(lex, "setenv") || str_equals(lex, "unsetenv"))
		return (TRUE);
	else if (str_equals(lex, "cd") || str_equals(lex, "alias"))
		return (TRUE);
	return (FALSE);
}
/**
 * alias_exec - alias execute
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int alias_exec(simple_command_t *command, sh_session_t *session)
{
	token_node_t *tok_node = NULL;
	alias_t *al = NULL;
	int index = -1;
	size_t len;
	char *alias = NULL, *name, *value;
	/**alias without arg**/
	if (command->args == NULL || command->args->head == NULL)
	{
		print_all_alias(session->alias_list);
		return (0);
	}
	tok_node = command->args->head;
	while (tok_node != NULL)
	{
		len = str_len(tok_node->token->lexeme);
		index = index_of(tok_node->token->lexeme, 0, len - 1, '=');
		if (index >= 0)
		{
			name = sub_str(tok_node->token->lexeme, 0, index - 1);
			value = sub_str(tok_node->token->lexeme, index + 1, len - 1);
			add_or_update_alias_list(&session->alias_list, name, value);
		}
		else
		{
			al = find_alias(tok_node->token->lexeme, session->alias_list);
			if (al != NULL)
			{
				alias = concat_strs(4, al->name, "='", al->value, "'\n");
				_puts(alias);
				free(alias);
			}
		}
		tok_node = tok_node->next;
	}
	return (0);
}
/**
 * setenv_exec - execute setenv
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int setenv_exec(simple_command_t *command, sh_session_t *session)
{
	size_t len = 0;
	token_node_t *node = NULL;
	char *lin = NULL;
	int ret = 0;

	if (command->args == NULL || command->args->head == NULL)
		return (0);
	node = command->args->head;
	while (node != NULL)
	{
		len++;
		node = node->next;
	}
	if (len > 2)
	{
		_fputs(session->sh_name, STDERR_FILENO);
		_fputs(": ", STDERR_FILENO);
		lin = int_to_str(command->cmd->line);
		_fputs(lin, STDERR_FILENO);
		if (lin != NULL)
		{
			free(lin);
			lin = NULL;
		}
		_fputs(": setenv: too many arguments", STDERR_FILENO);
		_fputc('\n', STDERR_FILENO);
		return (1);
	}
	if (len == 2)
	ret = _setenv(command->args->head->token->lexeme,
			command->args->head->next->token->lexeme, TRUE, &session->env_var_lst);
	return (ret);
}
/**
 * cd_exec - execute cd
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int cd_exec(simple_command_t *command, sh_session_t *session)
{
	char *pwd = NULL, *dir = NULL, *tmp = NULL;
	int ret = 0;
	size_t len = 0;

	pwd = _getenv("PWD", session->env_var_lst);
	if (command->args != NULL && command->args->head != NULL)
	{
		dir = command->args->head->token->lexeme;
		len = str_len(dir);
	}
	if (dir == NULL || str_equals(dir, "~") == TRUE)
		ret = _cd_home(session, pwd);
	else if (str_equals(dir, "-") == TRUE)
		ret = _cd_oldpwd(session, pwd);
	else if (str_equals(dir, ".") || str_equals(dir, ".."))
		ret = _cd(dir, session, pwd);
	else
	{
		if (dir[len - 1] != '/')
		{
			tmp = concat_str(dir, "/");
			dir = tmp;
		}
		if (access(dir, F_OK) == 0)
			ret = _cd(dir, session, pwd);
		else
		{
			print_cd_error(session->sh_name, command->args->head->token->line,
					command->args->head->token->lexeme);
			ret = 2;
		}
	}
	if (pwd != NULL)
		free(pwd);
	if (tmp != NULL)
		free(tmp);
	return (ret);
}

