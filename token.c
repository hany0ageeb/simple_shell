#include "token.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
/**
 * create_token - create token object
 * @lexeme: lexeme
 * @line: line
 * @type: token type
 * Return: a pointer to newly created token or NULL
 */
token_t *create_token(const char *lexeme, size_t line, token_type_t type)
{
	struct token *token = NULL;

	token = malloc(sizeof(struct token));
	if (token != NULL)
	{
		token->lexeme = copy_str(lexeme);
		token->line = line;
		token->type = type;
	}
	return (token);
}
/**
 * copy_token - create a copy from another token object
 * @src: source token object to copy
 * Return: the new copy of the object or NULL
 */
struct token *copy_token(const struct token *src)
{
	token_t *cpy = NULL;

	if (src != NULL)
	{
		cpy = create_token(src->lexeme, src->line, src->type);
	}
	return (cpy);
}
/**
 * free_token - free allocated token memory
 * @token: a token to free its memory
 * Return: void
 */
void free_token(struct token **token)
{
	if (token == NULL)
	{
		errno = EINVAL;
		return;
	}
	if (*token != NULL)
	{
		if ((*token)->lexeme != NULL)
		{
			free((*token)->lexeme);
			(*token)->lexeme = NULL;
		}
		free(*token);
		*token = NULL;
	}
	errno = 0;
}
/**
 * create_token_node - create a token node object
 * @token: token
 * Return: the newly created token_node object OR NULL
 */
struct token_node *create_token_node(struct token *token)
{
	struct token_node *node = NULL;

	node = malloc(sizeof(struct token_node));
	if (node != NULL)
	{
		node->token = token;
		node->next = NULL;
	}
	return (node);
}
/**
 * free_token_node - free memory allocated to token node
 * @pnode: token node object
 * Return: void
 */
void free_token_node(struct token_node **pnode)
{
	if (pnode == NULL)
	{
		errno = EINVAL;
	}
	if (*pnode != NULL)
	{
		if ((*pnode)->token != NULL)
		{
			free_token(&(*pnode)->token);
		}
		free(*pnode);
		*pnode = NULL;
	}
	errno = 0;
}

