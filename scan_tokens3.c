#include "shell.h"
#include "token.h"

/**
 * add_token - add token to a list of tokens
 * @lst: list of tokens
 * @lexeme: lexeme
 * @line: line number
 * @type: token type
 * Return: the newly added token
 */
token_t *add_token(token_list_t **lst, const char *lexeme,
		size_t line, token_type_t type)
{
	struct token *token = NULL;

	if (*lst == NULL)
		*lst = create_token_list();
	token = create_token(lexeme, line, type);
	if (token != NULL)
		add_token_to_list(*lst, token);
	return (token);
}

