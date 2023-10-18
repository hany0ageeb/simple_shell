#include "../simple_command.h"
#include <stdlib.h>
#include <stdio.h>

static void test_create_simple_command()
{
	simple_command_t *command;
	token_list_t *args = create_token_list();
	token_node_t *v;
	add_token_to_list(args, create_token("-la", 1, WORD));
	printf("==>test create_simple_command function\n");
	command = create_simple_command(create_token("ls", 1, WORD), args);
	if (command != NULL)
	{
		printf("command cmd token: [lexeme=%s, line=%lu]\n", command->cmd->lexeme, command->cmd->line);
		v = command->args != NULL ? command->args->head : NULL;
		while (v != NULL)
		{
			printf("arg: %s\n", v->token->lexeme);
			v = v->next;
		}
		free_token_list(&command->args);
		free_token(&command->cmd);
		free(command);
	}
	printf("==>end test create_simple_command function\n");
}
static void test_create_binary_command()
{
	simple_command_t *command;
	simple_command_t *left;
	simple_command_t *right;
	token_t *op;
	left = create_simple_command(create_token("ls", 1, WORD), NULL);
	right = create_simple_command(create_token("pwd", 1, WORD), NULL);
	op = create_token("&&", 1, AMP_AMP);
	printf("==>test create_binary_command function\n");
	command = create_binary_command(left, right, op);
	if (command != NULL)
	{
		printf("===>command was created\n");
		printf("===left operand=%s, right operand=%s, operator=%s\n",
				command->left->cmd->lexeme, command->right->cmd->lexeme, command->op->lexeme);
		free_simple_command(&command);
	}
	printf("==>end test create_binary_command function\n");
}
static void test_free_simple_command()
{
	simple_command_t *command;
	simple_command_t *left;
	simple_command_t *right;
	token_t *op;

	left = create_simple_command(create_token("ls", 1, WORD), NULL);
        right = create_simple_command(create_token("pwd", 1, WORD), NULL);
        op = create_token("&&", 1, AMP_AMP);
	printf("==>test free_simple_command function\n");
	command = create_binary_command(left, right, op);
	free_simple_command(&command);
	printf("==>test free_simple_command function end\n");
}
int main(void)
{
	printf("===========test simple_command.c=================\n");
	test_create_simple_command();
	printf("==================================================\n");
	test_create_binary_command();
	printf("==================================================\n");
	test_free_simple_command();
	printf("==================================================\n");
	printf("===========end test simple_command.c==============\n");
	exit(EXIT_SUCCESS);
}

