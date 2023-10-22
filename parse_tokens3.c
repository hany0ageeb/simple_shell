#include "shell.h"
#include "str_list.h"
#include "string.h"
#include "io.h"
#include "env.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * print_not_found_err - print no such file to stderr
 * @prog: shell name
 * @cmd: command token
 * Return: void
 */
void print_not_found_err(const char *prog, const token_t *cmd)
{
	char *msg = NULL, *ln = NULL;

	ln = int_to_str(cmd->line);
	msg = concat_strs(6, prog, ": ", ln, ": ", cmd->lexeme, ": not found\n");
	if (msg != NULL)
	{
		_fputs(msg, STDERR_FILENO);
		free(msg);
	}
	if (ln != NULL)
		free(ln);
}
/**
 * is_valid_token_arg - check if token is valid as command argument
 * @token: token
 * Return: TRUE if valid otherwise FALSE
 */
bool_t is_valid_token_arg(const token_t *token)
{
	if (token == NULL)
		return (FALSE);
	if (token->type == SEMI_COLON || token->type == NEW_LINE)
		return (FALSE);
	if (token->type == WORD || token->type == NUMBER)
		return (TRUE);
	if (token->type == DOLLAR_DOLLAR || token->type == DOLLAR_QUESTION)
		return (TRUE);
	if (token->type == VARIABLE)
		return (TRUE);
	return (FALSE);
}
/**
 * search_for_cmd - search for cmd file
 * @start: start node
 * @session: session
 * Return: cmd
 */
token_t *search_for_cmd(token_node_t *start, sh_session_t *session)
{
	char **paths = NULL;
	char *full_path = NULL;
	token_t *cmd_tok = NULL;

	replace_cmd_tok_var(start->token, session);
	if (contains_char(start->token->lexeme, '/') == TRUE)
	{
		if (access(start->token->lexeme, X_OK) == 0)
		{
			cmd_tok = copy_token(start->token);
			return (cmd_tok);
		}
		else
		{
			print_not_found_err(session->sh_name, start->token);
			session->status = 127;
			return (NULL);
		}
	}
	else
	{
		paths = get_paths(session->env_var_lst);
		full_path = find_full_path(start->token->lexeme, paths);
		if (paths != NULL)
			free_str_list(&paths);
		if (full_path == NULL)
		{
			session->status = 127;
			print_not_found_err(session->sh_name, start->token);
			return (NULL);
		}
		cmd_tok = create_token(full_path, start->token->line, WORD);
		free(full_path);
		return (cmd_tok);
	}
}
/**
 * replace_cmd_tok_var - replace var with its value if possible
 * @cmd_token: command token
 * @session: session
 * Return: void
 */
void replace_cmd_tok_var(token_t *cmd_token, sh_session_t *session)
{
	char *var_name = NULL, *var_value = NULL;

	if (cmd_token != NULL)
	{
		if (cmd_token->type == VARIABLE)
		{
			var_name = sub_str(cmd_token->lexeme, 1, str_len(cmd_token->lexeme) - 1);
			var_value = _getenv(var_name, session->env_var_lst);
			if (var_value != NULL)
			{
				free(cmd_token->lexeme);
				cmd_token->lexeme = var_value;
				cmd_token->type = WORD;
			}
			if (var_name != NULL)
				free(var_name);
		}
	}
}

