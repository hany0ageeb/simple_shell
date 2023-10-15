#include "shell.h"
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>>
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
int execute_command(simple_command_t *command, sh_session_t *session)
{
	pid_t child_id;

	if (command == NULL)
		return (-1);
	if (command->op == NULL)
	{
		child_id = fork();
		if (child_id == -1)
		{
			perror("fork");
			return (-1);
		}
		else if (child_id == 0)
		{
			if (command->is_builtin)
			{

			}
			else
			{
				if (execve(command->cmd->lexeme, get_args(command), session->env_var_lst) == -1)
				{
					perror("execve");
					return (-1);
				}
			}
		}
		else
		{
			wait(&session->status);
			return (session->status);
		}
	}
	else
	{

	}
}
simple_command_t *get_simple_command(token_node_t *start, token_node_t *end)
{
	token_node_t *op_node = NULL, *e = NULL;
	token_t *cmd_token = NULL;
	token_list_t *args = NULL;
	simple_command_t *command = NULL, *left = NULL, *right = NULL;

	if (start == NULL)
		return (NULL);
	op_node = get_op(start, end);
	if (op_node != NULL)
	{
		e = start;
		while (e != NULL && e->next != NULL &&e->next != op_node)
			e = e->next;
		left = get_simple_command(start, e);
		right = get_simple_command(op_node->next, end);
		command = create_binary_command(left, right, op_node->token);
		command->is_builtin = FALSE;
	}
	else
	{
		if (contains_char(start->token->lexeme, '/'))
		{
			cmd_token = copy_token(start->token);
			command->is_builtin = FALSE;
		}
		else if (is_builtin_cmd(start->token->lexeme))
		{
			command->is_builtin = TRUE;
		}
		else
		{
			command->is_builtin = TRUE;
		}
		start = start->next;
		if (start != NULL && start != end)
		{
			args = copy_token_list(start, end);
		}
		command = create_simple_command(cmd_token, args);
	}
	return (command);
}
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
		while (current != NULL && current->next != NULL && current->next->token->type != NEW_LINE && current->next->token->type != SEMI_COLON)
			current = current->next;
		command = get_simple_command(start, current);
		if (command != NULL && command->execute != NULL)
		{
			execute_result = command->execute(command, session);
		}
		if (current != NULL && current->next != NULL)
		{
			start = current->next->next;
			current = current->next->next;
		}
	}
	return (execute_result);
}
