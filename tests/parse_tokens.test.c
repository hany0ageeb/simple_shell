#include "../shell.h"
#include "../session.h"
#include <stdlib.h>
#include <stdio.h>

void test_get_op()
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
void print_simple_command(simple_command_t *command)
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
void test_replace_variables(char **envp)
{
	token_list_t *tokens_lst = create_token_list();
	sh_session_t *session = create_session("/hsh", envp);
	token_node_t *v = NULL;
	printf("==>testsing replace_variables\n");
	tokens_lst->head = create_token_node(create_token("$$", 1, DOLLAR_DOLLAR));
	tokens_lst->head->next = create_token_node(create_token("$?", 1, DOLLAR_QUESTION));
	printf("Before replacing\n");
	v = tokens_lst->head;
	while (v != NULL)
	{
		printf("%s\n", v->token->lexeme);
		v = v->next;
	}
	replace_variables(tokens_lst, session);
	printf("After replaacing\n");
	v = tokens_lst->head;
	while (v != NULL)
	{
		printf("%s\n", v->token->lexeme);
		v = v->next;
	}
	free_token_list(&tokens_lst);
	free_session(&session);
}
void test_get_simple_command(char **argv, char **envp)
{
	const char *src1 = "pwd";
	const char *src2 = "ls -la tests";
	const char *src3 = "pwd && ls";
	const char *src4 = "exit";
	token_list_t *tokens_lst = NULL;
	token_list_t *lst = NULL;
	token_node_t *start, *end;
	simple_command_t *command = NULL;
	sh_session_t *session = NULL;
	int ret;
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
			if (command->execute != NULL)
			{
				ret = command->execute(command, session);
				printf("ret=%d\n", ret);
			}
			else
			{
				printf("?????????????Null Execute function\n");
			}
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
			printf("=====>executing command\n");
			ret = command->execute(command, session);
			printf("ret=%d\n", ret);
			printf("=====>end executing command\n");
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
			ret = command->execute(command, session);
			printf("ret=%d\n", ret);
			free_simple_command(&command);
		}
	}
	printf("================================\n");
	printf("==>use case: src = %s\n", src4);
	if (scan_tokens(src4, &lst, "$$"))
	{
		start = end = lst->head;
		while (end != NULL && end->token->type != SEMI_COLON && end->token->type != NEW_LINE)
			end = end->next;
		command = get_simple_command(start, end, session);
		if (command != NULL)
		{
			print_simple_command(command);
			ret = command->execute(command, session);
			printf("ret=%d\n", ret);
			free_simple_command(&command);
		}
	}
	free_token_list(&lst);
	free_token_list(&tokens_lst);
	free_session(&session);
	printf("====================\n");

}
void test_builtin_commands(char **envp)
{
	const char *src1 = "exit 98";
	token_list_t *lst = NULL;
	simple_command_t *command = NULL;
	token_node_t *start = NULL, *end = NULL;
	sh_session_t *session = create_session("$$", envp);
	printf("==>src = %s\n", src1);
	if (scan_tokens(src1, &lst, "$$"))
	{
		start = end = lst->head;
		while (end != NULL && end->token->type != SEMI_COLON && end->token->type != NEW_LINE)
			end = end->next;
		command = get_simple_command(start, end, session);
		if (command != NULL)
		{
			if (command->execute != NULL)
			{
				command->execute(command, session);
			}
			free_simple_command(&command);
		}
	}
	free_token_list(&lst);
	free_session(&session);
}
int main(int argc, __attribute__((unused))char **argv, char **envp)
{
	printf("=============test parse_tokens.c===============\n");
	printf("argc = %d\n", argc);
	test_get_op();
	printf("================================================\n");
	test_replace_variables(envp);
	printf("=================================================\n");
	test_get_simple_command(argv, envp);
	test_builtin_commands(envp);
	printf("=============end test parse_token.c============\n");
	exit(EXIT_SUCCESS);
}

