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
	const char *src7 = "ls";
	const char *src8 = "echo $$";
	const char *src9 = "/bin/ls ;\n";
	struct token_list *lst = NULL;

	if (scan_tokens(src, &lst, "/bin/sh"))
	{
		printf("==>src = %s\n", src);
		print_token_list(lst);
	}
	printf("===============================================\n");
	if (scan_tokens(src2, &lst, "/bin/sh"))
	{
		printf("==>src = %s\n", src2);
		print_token_list(lst);
	}
	printf("===============================================\n");
	if (scan_tokens(src3, &lst, "/bin/sh"))
	{
		printf("==>src = %s\n", src3);
		print_token_list(lst);
	}
	printf("==================================================\n");
	if (scan_tokens(src4, &lst, "/bin/sh"))
	{
		printf("==>src = %s\n", src4);
		print_token_list(lst);
	}
	printf("===================================================\n");
	if (scan_tokens(src5, &lst, "/bin/sh"))
	{
		printf("==>src = %s\n", src5);
		print_token_list(lst);
	}
	printf("=========================================================\n");
	if (scan_tokens(src6, &lst, ""))
	{
		printf("==>src = %s\n", src6);
		print_token_list(lst);
	}
	printf("===========================================================\n");
	if (scan_tokens(src7, &lst, "/bin/sh"))
	{
		printf("==>src = %s\n", src7);
		print_token_list(lst);
	}
	printf("===========================================================\n");
	if (scan_tokens(src8, &lst, "/bin/sh"))
	{
		printf("==>src8 = %s\n", src8);
		print_token_list(lst);
	}
	printf("===================================================\n");
	if (scan_tokens(src9, &lst, "/bin/sh"))
	{
		printf("===>src9 = %s\n", src9);
		print_token_list(lst);
	}
	free_token_list(&lst);
	exit(EXIT_SUCCESS);
}

