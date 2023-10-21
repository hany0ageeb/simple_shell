#include "shell.h"
#include "string.h"
#include "io.h"
#include "str_list.h"
#include "builtin.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * execute_binary_command - execute binary command
 * @command: command
 * @session: session
 * Return: 0 on success
 */
int execute_binary_command(simple_command_t *command, sh_session_t *session)
{
	int ret = -1;

	ret = command->left->execute(command->left, session);
	if ((ret == 0 && command->op->type == AMP_AMP) ||
			(ret != 0 && command->op->type == PIPE_PIPE))
		ret = command->right->execute(command->right, session);
	return (ret);
}
/**
 * execute_command - execute a command
 * @command: simple_command object
 * @session: shell session object
 * Return: execution return value
 */
int execute_command(simple_command_t *command, sh_session_t *session)
{
	pid_t child_id;
	int ret = -1;
	char **args = NULL;

	if (command == NULL)
		return (-1);
	if (command->op != NULL)
	{
		ret = execute_binary_command(command, session);
		session->status = ret;
	}
	else
	{
		if (command->is_builtin)
			return (command->execute(command, session));
		child_id = fork();
		if (child_id == -1)
			return (-1);
		else if (child_id == 0)
		{
			args = get_args(command);
			if (execve(command->cmd->lexeme, args, session->env_var_lst) == -1)
			{
				if (args != NULL)
					free_str_list(&args);
				return (-1);
			}
		}
		else
		{
			waitpid(child_id, &ret, WUNTRACED);
			if (WIFEXITED(ret))
				session->status = WEXITSTATUS(ret);
		}
	}
	if (args != NULL)
		free_str_list(&args);
	return (session->status);
}
/**
 * make_simple_command - create non binary command
 * @start: starte node
 * @end: end node
 * @session: shell session
 * Return: NULL or created command
 */
simple_command_t *make_simple_command(token_node_t *start, token_node_t *end,
		sh_session_t *session)
{
	simple_command_t *command = NULL;
	bool_t builtin_command = FALSE;
	token_t *cmd_token = NULL;
	token_list_t *args = NULL;

	if (is_builtin_cmd(start->token->lexeme))
	{
		cmd_token = copy_token(start->token);
		builtin_command = TRUE;
	}
	else
	{
		builtin_command = FALSE;
		cmd_token = search_for_cmd(start, session);
		if (cmd_token == NULL)
			return (NULL);
	}
	args = get_args_list(start, end);
	if (args != NULL)
		replace_variables(args, session);
	command = create_simple_command(cmd_token, args);
	if (command != NULL)
	{
		command->is_builtin = builtin_command;
		set_command_execute(command);
	}
	return (command);
}
/**
 * get_simple_command - convert tokens to simple_command
 * @start: start node
 * @end: end node
 * @session: shell session
 * Return: simple_command object on success or NULL if failed.
 */
simple_command_t *get_simple_command(token_node_t *start, token_node_t *end,
		sh_session_t *session)
{
	token_node_t *op_node = NULL, *e = NULL;
	simple_command_t *command = NULL, *left = NULL, *right = NULL;

	if (start == NULL)
		return (NULL);
	op_node = get_op(start, end);
	if (op_node != NULL)
	{
		e = start;
		while (e != NULL && e->next != NULL && e->next != op_node)
		{
			e = e->next;
		}
		left = get_simple_command(start, e, session);
		right = get_simple_command(op_node->next, end, session);
		command = create_binary_command(left, right, copy_token(op_node->token));
		command->is_builtin = FALSE;
		command->execute = execute_command;
	}
	else
	{
		command = make_simple_command(start, end, session);
	}
	return (command);
}
/**
 * parse_tokens - parse tokens
 * @lst: list of tokens
 * @session: shell session
 * Return: 0 on success
 */
int parse_tokens(const token_list_t *lst, sh_session_t *session)
{
	token_node_t *start, *current;
	simple_command_t *command = NULL;
	int execute_result = 0;

	if (lst == NULL || lst->head == NULL)
		return (0);
	start = current = lst->head;
	while (current != NULL)
	{
		while (current != NULL &&
				current->token->type != NEW_LINE &&
				current->token->type != SEMI_COLON)
			current = current->next;
		if (current != NULL)
		{
			command = get_simple_command(start, current, session);
			if (command != NULL && command->execute != NULL)
			{
				execute_result = command->execute(command, session);
				free_simple_command(&command);
			}
			current = current->next;
			while (current != NULL &&
					(current->token->type == NEW_LINE || current->token->type == SEMI_COLON))
				current = current->next;
			start = current;
		}
	}
	return (execute_result);
}

