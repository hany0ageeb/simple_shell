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
		ret = command->left->execute(command->left, session);
		if ((ret == 0 && command->op->type == AMP_AMP) ||
				(ret == -1 && command->op->type == PIPE_PIPE))
		{
			ret = command->right->execute(command->right, session);
		}
		session->status = ret;
	}
	else
	{
		if (command->is_builtin)
		{
			printf("built in command\n");
			return (command->execute(command, session));
		}
		child_id = fork();
		if (child_id == -1)
		{
			perror("fork");
			return (-1);
		}
		else if (child_id == 0)
		{
			args = get_args(command);
			if (execve(command->cmd->lexeme, args, session->env_var_lst) == -1)
			{
				if (args != NULL)
					free_str_list(args);
				perror("execve");
				return (-1);
			}
		}
		else
		{
			wait(&session->status);
			ret = session->status;
		}
	}
	if (args != NULL)
	{
		free_str_list(args);
	}
	return (ret);
}
static void print_not_found_err(const char *lexeme, size_t line,
		const char *prog)
{
	_puts(prog);
	_puts(": ");
	_puts(int_to_str(line));
	_puts(": ");
	_puts(lexeme);
	_puts(": not found");
	_putc('\n');
}
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
void replace_variables(token_list_t *args, sh_session_t *session)
{
	token_node_t *v = NULL;
	char *var_value = NULL;

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
				if (v->token->lexeme != NULL)
				{
					free(v->token->lexeme);
					v->token->lexeme = NULL;
				}
				v->token->lexeme = NULL;
				var_value = _getenv(v->token->lexeme, session->env_var_lst);
				if (var_value == NULL)
					v->token->lexeme = copy_str("");
				else
					v->token->lexeme = var_value;
				v->token->type = WORD;
			}
			v = v->next;
		} while (v != NULL);
	}
}
simple_command_t *make_simple_command(token_node_t *start, token_node_t *end, sh_session_t *session)
{
	simple_command_t *command = NULL;
	bool_t builtin_command = FALSE;
	char *full_path = NULL;
	char **paths = NULL;
	token_t *cmd_token = NULL;
	token_list_t *args = NULL;

	if (contains_char(start->token->lexeme, '/'))
	{
		if (access(start->token->lexeme, X_OK))
		{
			cmd_token = copy_token(start->token);
			builtin_command = FALSE;
		}
		else
		{
			print_not_found_err(session->sh_name, start->token->line,
					start->token->lexeme);
			return (NULL);
		}
	}
	else if (is_builtin_cmd(start->token->lexeme))
	{
		cmd_token = copy_token(start->token);
		builtin_command = TRUE;
	}
	else
	{
		builtin_command = FALSE;
		paths = get_paths(session->env_var_lst);
		full_path = find_full_path(start->token->lexeme, paths);
		if (paths != NULL)
			free_str_list(paths);
		if (full_path == NULL)
		{
			print_not_found_err(session->sh_name, start->token->line,
					start->token->lexeme);
			return (NULL);
		}
		cmd_token = create_token(full_path, start->token->line, WORD);
		free(full_path);
		full_path = NULL;
	}
	if (start != NULL && start != end)
	{
		start = start->next;
		if (start != NULL && start != end)
		{
			args = copy_token_list(start, end);
		}
	}
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
		command = create_binary_command(left, right, op_node->token);
		command->is_builtin = FALSE;
		command->execute = execute_command;
	}
	else
	{
		command = make_simple_command(start, end, session);
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
		while (current != NULL && current->next != NULL &&
				current->next->token->type != NEW_LINE &&
				current->next->token->type != SEMI_COLON)
			current = current->next;
		command = get_simple_command(start, current, session);
		if (command != NULL && command->execute != NULL)
		{
			execute_result = command->execute(command, session);
			session->status = execute_result;
		}
		if (current != NULL && current->next != NULL)
		{
			start = current->next->next;
			current = current->next->next;
		}
	}
	return (execute_result);
}

