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
	const char *src = "ls -la test";
	const char *src2 = "exit 98";
	const char *src3 = "ls|;";
	const char *src4 = "ls&&ls";
	const char *src5 = "ls&&;";
	const char *src6 = "ls;&&ls;";
	struct token_list *lst = NULL;

	if (scan_tokens(src, &lst, "/bin/sh"))
	{
		print_token_list(lst);
	}
	printf("===============================================\n");
	if (scan_tokens(src2, &lst, "/bin/sh"))
	{
		print_token_list(lst);
	}
	printf("===============================================\n");
	if (scan_tokens(src3, &lst, "/bin/sh"))
	{
		print_token_list(lst);
	}
	printf("==================================================\n");
	if (scan_tokens(src4, &lst, "/bin/sh"))
	{
		print_token_list(lst);
	}
	printf("===================================================\n");
	if (scan_tokens(src5, &lst, "/bin/sh"))
	{
		print_token_list(lst);
	}
	printf("=========================================================\n");
	if (scan_tokens(src6, &lst, ""))
	{
		print_token_list(lst);
	}
	free_token_list(&lst);
	exit(EXIT_SUCCESS);
}

