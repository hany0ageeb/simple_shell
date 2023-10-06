#include "../token.h"
#include "../shell.h"
#include <stdio.h>
#include <stdlib.h>
static void print_token_list(struct token_list *lst)
{
	size_t count = 1;
	struct token_node *node;

	if (lst != NULL)
	{
		node = lst->head;
		while (node != NULL)
		{
			printf("%lu: ", count);
			printf("lexeme=%s, line=%lu, type=%d\n", node->token->lexeme, node->token->line, node->token->type);
			node = node->next;
			count++;
		}
	}
}
int main(void)
{
	const char *src = "ls -la tests\n";
	const char *src2 = "exit 98";
	struct token_list *lst = scan_tokens(src);

	if (lst != NULL)
	{
		print_token_list(lst);
		free_token_list(&lst);
	}
	else
	{
		printf("Error: Invalid Token List which is null\n");
	}
	lst = scan_tokens(src2);
	if (lst != NULL)
	{
		print_token_list(lst);
		free_token_list(&lst);
	}
	else
	{
		printf("Error: Invalid NULL token list object\n");
	}
	exit(EXIT_SUCCESS);
}

