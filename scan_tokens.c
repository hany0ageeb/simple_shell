#include "shell.h"
#include "string.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 * report_error - report syntax errors
 * @argv0: program name as passed to argv of main function
 * @pre_token: previous token
 * @lexeme: current lexeme
 * @line: line number
 * @c: charachter
 * @next_char: next charachter
 * Return: void
 */
void report_error(const char *argv0, token_t *pre_token,
		const char *lexeme, size_t line, const char c, const char next_char)
{
	char *tmp = NULL;
	enum token_type type;

	if (pre_token == NULL)
	{
		if (next_char == '|' && c == '|')
			tmp = copy_str("||");
		else if (next_char == ';' && c == ';')
			tmp = copy_str(";;");
		else if (next_char == '>' && c == '>')
			tmp = copy_str(">>");
		else if (next_char == '&' && c == '&')
			tmp = copy_str("&&");
		printf("%s: %lu Syntax error: \"%s\" unexpected\n", argv0, line,
				tmp != NULL ? tmp : lexeme);
	}
	else
	{
		if (c == '\n')
		{
			printf("%s: %lu Syntax error: \"%s\" unexpected\n", argv0, line,
					pre_token->lexeme);
		}
		else
		{
			type = pre_token->type;
			if (type != WORD && type != NUMBER &&
					(c == '|' || c == '&' || c == '>' || c == '<' || c == ';'))
			{
				tmp = concat_str(pre_token->lexeme, lexeme);
				printf("%s: %lu Syntax error: \"%s\" unexpected\n", argv0, line, tmp);
			}
			else
				printf("%s: %lu Syntax error: \"%s\" unexpected\n", argv0, line, lexeme);
		}
	}
	if (tmp != NULL)
		free(tmp);
}
/**
 * is_valid_token - check if the current token is valid syntactically
 * @pre_token: previous token
 * @c: current char
 * Return: TRUE or FALSE
 */
bool_t is_valid_token(const token_t *pre_token, const char c)
{
	enum token_type type;

	if (c == ';')
		type = SEMI_COLON;
	else if (c == '|')
		type = PIPE;
	else if (c == '&')
		type = AMPERSAND;
	else if (c == '>')
		type = GREATER_THAN;
	else if (c == '<')
		type = LESS_THAN;
	else if (c == '\n')
		type = NEW_LINE;
	else if (c == '\t' || c == '\r' || c == ' ' || '#')
		return (TRUE);
	else if (is_alpha_numeric(c))
		return (TRUE);
	if ((pre_token == NULL ||
				pre_token->type == NEW_LINE || pre_token->type == SEMI_COLON)
			&& (type != WORD && type != NUMBER && type != NEW_LINE))
		return (FALSE);
	if (pre_token != NULL && (pre_token->type != WORD &&
				pre_token->type != NUMBER && pre_token->type != NEW_LINE) &&
			type != WORD && type != NUMBER && type != NEW_LINE)
		return (FALSE);
	return (TRUE);
}
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
/**
 * match - match expected with next char in src
 * @expected: expected char in src
 * @src: source code
 * @pcurrent: current char
 * Return: TRUE if expected equals src[current + 1] otherwise FALSE
 */
bool_t match(char expected, const char *src, size_t *pcurrent)
{
	if (src[*pcurrent] == '\0')
		return (FALSE);
	if (src[*pcurrent] != expected)
		return (FALSE);
	(*pcurrent)++;
	return (TRUE);
}
/**
 * scan_tokens - scan tokens
 * @src: source code
 * @lst: a list of tokens
 * @argv0: argv[0] passed to main function.
 * Return: TRUE if no syntax errors otherwise FALSE
 */
bool_t scan_tokens(const char *src, token_list_t **lst,
		const char *argv0)
{
	size_t start = 0, current = 0, line = 1, len;
	char c;
	struct token *pre_token = NULL;

	if (lst == NULL)
	{
		errno = EINVAL;
		return (FALSE);
	}
	len = str_len(src);
	if (*lst != NULL)
		free_token_list(lst);
	while (current < len && src[current] != '\0')
	{
		c = src[current];
		current++;
		if (!is_valid_token(pre_token, c))
		{
			report_error(argv0, pre_token, "|", line, c, src[current + 1]);
			return (FALSE);
		}
		if (c == '\t' || c == '\r' || c == ' ')
			;
		else if (c == '\n' || c == '|' || c == ';'
				|| c == '&' || c == '>' || c == '<' || c == '$')
			consume_token(src, c, &pre_token, lst, &current, &line);
		else if (c == '#')
			consume_comment(src, c, &current, &line);
		else if (is_alpha_numeric(c))
			consume_word_token(src, c, &current, lst, line, start, &pre_token);
		else
		{
			printf("%s: %lu Syntax error: \"%c\" unexpected\n", argv0, line, c);
			return (FALSE);
		}
		start = current;
	}
	return (TRUE);
}

