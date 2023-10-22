#include "shell.h"
#include "string.h"
#include "env.h"
#include "builtin.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * get_op - get op
 * @start: start node
 * @end: end node
 * Return: NULL or token_node
 */
token_node_t *get_op(token_node_t *start, token_node_t *end)
{
	token_node_t *node = NULL;

	while (start != NULL && start != end)
	{
		if (start->token->type == AMP_AMP || start->token->type == PIPE_PIPE)
		{
			if (node == NULL)
			{
				if (start->token->type == AMP_AMP)
				{
					node = start;
					break;
				}
				else if (start->token->type == PIPE_PIPE)
				{
					node = start;
				}
			}
			else
			{
				if (node->token->type == PIPE_PIPE && start->token->type == AMP_AMP)
				{
					node = start;
					break;
				}
			}
		}
		start = start->next;
	}
	return (node);
}
/**
 * set_command_execute - set command execute function
 * @command: command
 * Return: void
 */
void set_command_execute(simple_command_t *command)
{
	if (command->is_builtin == FALSE)
		command->execute = execute_command;
	else
	{
		if (str_equals(command->cmd->lexeme, "exit"))
			command->execute = exit_exec;
		else if (str_equals(command->cmd->lexeme, "cd"))
			command->execute = cd_exec;
		else if (str_equals(command->cmd->lexeme, "setenv"))
			command->execute = setenv_exec;
		else if (str_equals(command->cmd->lexeme, "unsetenv"))
			command->execute = unsetenv_exec;
		else if (str_equals(command->cmd->lexeme, "env"))
			command->execute = env_exec;
		else if (str_equals(command->cmd->lexeme, "alias"))
			command->execute = alias_exec;
		else
			command->execute = NULL;
	}
}
/**
 * search_for_var_value - search for variable value in env
 * @args: arguments list
 * @pv: v
 * @ppre_v: pre_v
 * @session: session
 * Return: void
 */
void search_for_var_value(token_list_t *args,
		token_node_t **pv, token_node_t **ppre_v, sh_session_t *session)
{
	char *var_name = NULL, *var_value = NULL;

	var_name = sub_str((*pv)->token->lexeme, 1,
			str_len((*pv)->token->lexeme) - 1);
	var_value = _getenv(var_name, session->env_var_lst);
	if (var_value == NULL)
	{
		if ((*ppre_v) != NULL)
			(*ppre_v)->next = (*pv)->next;
		if ((*pv) == args->head)
		{
			free_token_node(pv);
			(*pv) = NULL;
			args->head = NULL;
			if (var_name != NULL)
				free(var_name);
			return;
		}
		else
		{
			free_token_node(pv);
		}
		(*pv) = (*ppre_v);
	}
	else
	{
		free((*pv)->token->lexeme);
		(*pv)->token->lexeme = var_value;
		(*pv)->token->type = WORD;
	}
	free(var_name);
}
/**
 * replace_variables - replace vars
 * @args: arguments list
 * @session: session
 * Return: void
 */
void replace_variables(token_list_t *args,
		sh_session_t *session)
{
	token_node_t *v = NULL, *pre_v = NULL;

	if (args != NULL && args->head != NULL)
	{
		v = args->head;
		do {
			if (v->token->type == DOLLAR_DOLLAR)
			{
				if (v->token->lexeme != NULL)
				{
					free(v->token->lexeme);
					v->token->lexeme = NULL;
				}
				v->token->lexeme = int_to_str(getpid());
				v->token->type = NUMBER;
			}
			else if (v->token->type == DOLLAR_QUESTION)
			{
				if (v->token->lexeme != NULL)
				{
					free(v->token->lexeme);
					v->token->lexeme = NULL;
				}
				v->token->lexeme = int_to_str(session->status);
				v->token->type = NUMBER;
			}
			else if (v->token->type == VARIABLE)
			{
				search_for_var_value(args, &v, &pre_v, session);
			}
			pre_v = v;
			if (v != NULL)
				v = v->next;
		} while (v != NULL);
	}
}
/**
 * get_args_list - get arguments list form token list
 * @start: start node
 * @end: end node
 * Return: token_list
 */
token_list_t *get_args_list(token_node_t *start, token_node_t *end)
{
	token_list_t *args = NULL;
	token_node_t *node = NULL;

	if (start == NULL)
		return (NULL);
	if (start == end)
		return (NULL);
	start = start->next;
	if (start == NULL || is_valid_token_arg(start->token) == FALSE)
		return (NULL);
	args = create_token_list();
	args->head = copy_token_node(start);
	if (start == end)
		return (args);
	start = start->next;
	node = args->head;
	while (start != NULL && start != end && is_valid_token_arg(start->token))
	{
		node->next = copy_token_node(start);
		node = node->next;
		start = start->next;
	}
	if (start != NULL && is_valid_token_arg(start->token))
	{
		node->next = copy_token_node(start);
	}
	return (args);
}

