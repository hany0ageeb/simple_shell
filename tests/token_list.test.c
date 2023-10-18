#include "../token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void test_create_token_list()
{
	struct token_list *lst = create_token_list();

	printf("==>create_token_list function test\n");
	if (lst != NULL)
	{
		if (lst->head != NULL)
		{
			printf("Invalid creation not seting head pointer to null!");
		}
		printf("token list object was created successfully\n");
		free(lst);
		lst = NULL;
	}
	else
	{
		printf("Error: Cannot create token list object\n");
	}
	printf("==>end create_token_list function test\n");
}
static void test_add_token_to_list()
{
	struct token_list *lst = create_token_list();
	struct token *token = NULL;

	printf("==>add_token_to_list function test\n");
	if (lst != NULL)
	{
		printf("===>created a list to add token to!\n");
		token = create_token(";", 1, SEMI_COLON);
		add_token_to_list(lst, token);
		if (lst->head != NULL)
		{
			printf("lst->head was set to a new token_node\n");
			if (lst->head->token != NULL)
			{
				printf("Token [lexeme=%s, line=%lu, type=%d\n", lst->head->token->lexeme,
						lst->head->token->line, lst->head->token->type);
				if (lst->head->next != NULL)
				{
					printf("Error: next was not null\n");
				}

			}
			else
                        {
                                printf("ERROR: lst->head->token is null\n");
                        }
			printf("adding a new token to non empty list\n");
			add_token_to_list(lst, create_token("|", 1, PIPE));
			if (lst->head->next != NULL)
			{
				if (lst->head->next->token != NULL)
				{
					printf("Token [lexeme = %s]\n", lst->head->next->token->lexeme);
				}
				free_token_node(&lst->head->next);
			}
			else
			{
				printf("Could not add the second token to list\n");
			}
			free_token_node(&lst->head);
		}
		else
		{
			printf("Failed in adding the token to list\n");
		}
		free(lst);
	}
	else
	{
		printf("Cannot create token list object\n");
	}
	printf("==>end add_token_to_list function tests\n");
}
static void test_free_token_list()
{
	struct token_list *lst = create_token_list();

	printf("==>tests free_token_list function\n");
	if (lst != NULL)
	{
		printf("create list to free\n");
		add_token_to_list(lst, create_token("ls", 1, WORD));
		add_token_to_list(lst, create_token("-la", 1, WORD));
		free_token_list(&lst);
		if (lst != NULL)
		{
			printf("Error: free_token_list did not set lst to null\n");
		}
		else
		{
			printf("lst was set to NULL as expected!\n");
		}
		lst = create_token_list();
		add_token_to_list(lst, create_token("ls", 1, WORD));
		free_token_list(&lst);
		if (lst != NULL)
			printf("Error: free_token_list\n");
		else
			printf("lst set to null\n");
	}
	else
	{
		printf("could not create a token list object for tests\n");
	}
	printf("==>end free_token_list function\n");
}
static void test_copy_token_node()
{
	token_node_t *src = create_token_node(create_token("||", 1, PIPE_PIPE));
	token_node_t *cpy;
	printf("===>test copy_token_node function\n");
	cpy = copy_token_node(src);
	if (cpy != NULL && src != NULL)
	{
		printf("Expected [%s] GOT [%s]\n", src->token->lexeme, cpy->token->lexeme);
	}
	if (cpy != NULL)
		free_token_node(&cpy);
	if (src != NULL)
		free_token_node(&src);
	printf("===>end test copy_token_node function\n");
}
static void test_copy_token_list()
{
	token_list_t *org = create_token_list();
	token_node_t *v;
	token_list_t *copy = NULL;
	printf("===>test copy_token_list function\n");
	org->head = create_token_node(create_token("ls", 1, WORD));
	org->head->next = create_token_node(create_token("-la", 1, WORD));
	org->head->next->next = create_token_node(create_token("tests", 1, WORD));
	printf("=Original List\n");
	v = org->head;
	while (v != NULL)
	{
		printf("Token[line=%lu, lexeme=%s]\n", v->token->line, v->token->lexeme);
		v = v->next;
	}
	printf("======================\n");
	copy = copy_token_list(org->head->next, NULL);
	v = copy->head;
	while (v != NULL)
	{
		printf("Token[line=%lu, lexeme=%s, type=%d]\n", v->token->line, v->token->lexeme, v->token->type);
		v = v->next;
	}
	printf("=======================\n");
	free_token_list(&copy);
	copy = copy_token_list(org->head, org->head->next);
	v = copy->head;
        while (v != NULL)
        {
                printf("Token[line=%lu, lexeme=%s, type=%d]\n", v->token->line, v->token->lexeme, v->token->type);
                v = v->next;
        }
	printf("=====================================\n");
	free_token_list(&copy);
	copy = copy_token_list(org->head, NULL);
	v = copy->head;
	while (v != NULL)
	{
		printf("Token [line=%lu, lexeme=%s]\n", v->token->line, v->token->lexeme);
		v = v->next;
	}
	free_token_list(&org);
	free_token_list(&copy);
	printf("===>end copy_token_list function\n");
}
int main(void)
{
	printf("==============token_list.c tests================\n");
	test_create_token_list();
	printf("================================================\n");
	test_add_token_to_list();
	printf("================================================\n");
	test_free_token_list();
	printf("=================================================\n");
	test_copy_token_node();
	printf("=================================================\n");
	test_copy_token_list();
	printf("=================================================\n");
	printf("==============end token_list.c tests============\n");
	exit(EXIT_SUCCESS);
}

