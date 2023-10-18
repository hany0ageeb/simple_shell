#include "../shell.h"
#include "../session.h"
#include <stdlib.h>
#include <stdio.h>

static void test_get_op()
{
	const char *src = "ls -la || pwd";
	token_list_t *tokens_lst = NULL;
	token_node_t *op;
	printf("==>test get_op function\n");
	if (scan_tokens(src, &tokens_lst, "$$"))
	{
		op = get_op(tokens_lst->head, NULL);
		if (op != NULL)
		{
			printf("Got [%s] Expected [%s]\n", op->token->lexeme, "||");
			if (op->next != NULL && op->next->token != NULL)
			{
				printf("Flollowed BY GOT [%s] EXPECTED [%s]\n", op->next->token->lexeme, "pwd");
			}
		}
		else
		{
			printf("GOT [NULL] EXPECTED [||]\n");
		}
	}
	free_token_list(&tokens_lst);
	printf("==>end test get_op function\n");
}
static void print_simple_command(simple_command_t *command)
{
	token_node_t *v = NULL;

	if (command != NULL && command->left != NULL)
	{
		printf("Left operand:\n");
		print_simple_command(command->left);
	}
	if (command != NULL && command->right != NULL)
	{
		printf("Right Operand:\n");
		print_simple_command(command->right);
	}
	if (command != NULL && command->op != NULL)
	{
		printf("Operator: %s\n", command->op->lexeme);
	}
	if (command != NULL && command->cmd != NULL)
	{
		printf("cmd: %s\n", command->cmd->lexeme);
	}
	if (command != NULL && command->args != NULL)
	{
		v = command->args->head;
		while (v != NULL)
		{
			printf("arg: %s, ", v->token->lexeme);
			v = v->next;
		}
		printf("\n");
	}
	printf("============printed command======\n");
}
static void test_get_simple_command(char **argv, char **envp)
{
	const char *src1 = "pwd";
	const char *src2 = "ls -la tests";
	const char *src3 = "gcc a.c -o a.out && ./a.out";
	token_list_t *tokens_lst = NULL;
	token_node_t *start, *end;
	simple_command_t *command = NULL;
	sh_session_t *session = NULL;
	session = create_session(argv[0], envp); 

	printf("==>use case: src = %s\n", src1);
	if (scan_tokens(src1, &tokens_lst, "$$"))
	{
		start = tokens_lst->head;
		end = tokens_lst->head;
		while (end != NULL && end->token->type != SEMI_COLON && end->token->type != NEW_LINE)
			end = end->next;
		command = get_simple_command(start, end, session);
		if (command != NULL)
		{
			print_simple_command(command);
			free_simple_command(&command);
		}
	}
	free_token_list(&tokens_lst);
	printf("===================>\n");
	printf("==>use case: src = %s\n", src2);
	if (scan_tokens(src2, &tokens_lst, "$$"))
	{
		start = tokens_lst->head;
		end = tokens_lst->head;
		while (end != NULL && end->token->type != SEMI_COLON && end->token->type != NEW_LINE)
			end = end->next;
		command = get_simple_command(start, end, session);
		if (command != NULL)
		{
			print_simple_command(command);
			free_simple_command(&command);
		}
	}
	free_token_list(&tokens_lst);
	printf("==========================\n");
	printf("==>use case: src = %s\n", src3);
	if (scan_tokens(src3, &tokens_lst, "$$"))
	{
		start = end = tokens_lst->head;
		while (end != NULL && end->token->type != SEMI_COLON && end->token->type != NEW_LINE)
			end = end->next;
		command = get_simple_command(start, end, session);
		if (command != NULL)
		{
			print_simple_command(command);
			/*free_simple_command(&command);*/
		}
	}
	free_token_list(&tokens_lst);
	free_session(&session);
	printf("====================\n");

}
int main(int argc, char **argv, char **envp)
{
	printf("=============test parse_tokens.c===============\n");
	printf("argc = %d\n", argc);
	test_get_op();
	printf("================================================\n");
	test_get_simple_command(argv, envp);
	printf("=============end test parse_token.c============\n");
	exit(EXIT_SUCCESS);
}

