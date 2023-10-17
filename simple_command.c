#include "simple_command.h"
#include "token.h"
#include "string.h"
#include <stdlib.h>
#include <errno.h>

simple_command_t *create_binary_command(simple_command_t *left, simple_command_t *right, token_t *op)
{
	simple_command_t *command = NULL;

	if (left == NULL || right == NULL || op == NULL)
	{
		errno = EINVAL;
		return (NULL);
	}
	command = malloc(sizeof(simple_command_t));
	if (command != NULL)
	{
		command->left = left;
		command->right = right;
		command->op = op;
		command->cmd = NULL;
		command->args = NULL;
	}
	return (command);
}
simple_command_t *create_simple_command(token_t *cmd, token_list_t *args)
{
	simple_command_t *command = NULL;

	if (cmd == NULL)
	{
		errno = EINVAL;
		return (command);
	}
	command = malloc(sizeof(simple_command_t));
	if (command != NULL)
	{
		command->cmd = cmd;
		command->args = args;
		command->left = NULL;
		command->right = NULL;
		command->op = NULL;
	}
	errno = 0;
	return (command);
}
void free_simple_command(simple_command_t **command)
{
	if (command == NULL)
	{
		errno = EINVAL;
		return;
	}
	if (*command != NULL)
	{
		if ((*command)->left != NULL)
		{
			free_simple_command(&(*command)->left);
			(*command)->left = NULL;
		}
		if ((*command)->right != NULL)
		{
			free_simple_command(&(*command)->right);
			(*command)->right = NULL;
		}
		if ((*command)->op != NULL)
		{
			free_token(&(*command)->op);
			(*command)->op = NULL;
		}
		if ((*command)->cmd != NULL)
		{
			free_token(&(*command)->cmd);
			(*command)->cmd = NULL;
		}
		if ((*command)->args != NULL)
		{
			free_token_list(&(*command)->args);
			(*command)->args = NULL;
		}
		free(*command);
		*command = NULL;
	}
	errno = 0;
}
char **get_args(const simple_command_t *command)
{
	char **args = NULL;
	token_node_t *node = NULL;
	size_t len = 0, i;

	if (command != NULL && command->cmd != NULL)
	{
		len++;
		if (command->args != NULL && command->args->head != NULL)
		{
			node = command->args->head;
			while (node != NULL)
			{
				len++;
				node = node->next;
			}
		}
	}
	if (len > 0)
	{
		args = malloc(sizeof(char *) * (len + 1));
		args[0] = copy_str(command->cmd->lexeme);
		i = 1;
		node = command->args != NULL && command->args->head != NULL ? command->args->head : NULL;
		while (i < len && node != NULL)
		{
			args[i] = copy_str(node->token->lexeme);
			i++;
			node = node->next;
		}
	}
	return (args);
}
