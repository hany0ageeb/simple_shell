#include "../token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void test_create_token()
{
	struct token *token = NULL;

	printf("==>test create_token function\n");
	token = create_token(";", 1, SEMI_COLON);
	if (token == NULL)
	{
		printf("==>could not create token\n");
		return;
	}
	printf("lexeme=%s, line=%lu type=%d\n", token->lexeme, token->line, token->type);
	if (token->lexeme != NULL)
		free(token->lexeme);
	free(token);
	printf("===end create_token function test=====\n");
}
static void test_free_token()
{
	struct token *token = create_token(";", 1, SEMI_COLON);

	printf("==>tests free_token function=====\n");
	free_token(&token);
	if (token == NULL)
	{
		printf("token is null after calling free_token\n");
	}
	else
	{
		printf("Failed in free token\n");
	}
	printf("===end free_token function tests\n");
}
static void test_create_token_node()
{
	struct token_node *node = create_token_node(create_token(";", 1, SEMI_COLON));
	struct token *token;

	printf("==>tests create_token_node function\n");
	if (node != NULL)
	{
		printf("token node created\n");
		token = node->token;
		if (token != NULL)
		{
			printf("token [lexeme=%s, line=%lu, type=%d\n", token->lexeme, token->line, token->type);
			free_token(&node->token);
		}
		free(node);
		node = NULL;
	}
	else
	{
		printf("Cannot create token_node\n");
	}
	printf("===end createtoken_node function tests==\n");
}
static void test_free_token_node()
{
	struct token_node *node = create_token_node(create_token(";", 1, SEMI_COLON));
	printf("===>test free_token_node function\n");
	if (node != NULL)
	{
		free_token_node(&node);
		if (node == NULL)
		{
			printf("All memory allocated was freed!\n");
		}
	}
	printf("====end free_token_node function\n");
}
static void test_copy_token()
{
	token_t *token = NULL;
	token_t *cpy;
	printf("===>tests copy_token function\n");
	printf("===>use case: when token is null will return null\n");
	cpy = copy_token(token);
	if (cpy != NULL)
	{
		printf("====>Failed: Expected NULL GOT NON NULL\n");
		free_token(&cpy);
	}
	printf("===>use case: when token is not null return an exact copy.\n");
	token = create_token(";", 1, SEMI_COLON);
	if (token != NULL)
	{
		cpy = copy_token(token);
		if (cpy != NULL)
		{
			if (cpy->line == token->line && cpy->type == token->type && strcmp(cpy->lexeme, token->lexeme) == 0)
			{
				printf("===>Passed\n");
			}
			else
			{
				printf("===>Failed\n");
				printf("Got [%s, %lu, %d] Expected [%s, %lu, %d]\n", token->lexeme, token->line, token->type, token->lexeme, token->line, token->type);
			}
			free_token(&cpy);
		}
		else
		{
			printf("Failed: GOT NULL COPY\n");
		}
		free_token(&token);
	}
	else
	{
		printf("===>Error While creating a token for testsing.\n");
	}
	printf("==>end copy_token function test\n");
}
int main(void)
{
	printf("============token.c tests===========\n");
	test_create_token();
	printf("==================================================\n");
	test_free_token();
	printf("==================================================\n");
	test_create_token_node();
	printf("==================================================\n");
	test_free_token_node();
	printf("==================================================\n");
	test_copy_token();
	printf("==================================================\n");
	printf("========token.c tests end===========\n");
	exit(EXIT_SUCCESS);
}

