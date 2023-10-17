#include "bool.h"
#include "token.h"
#include "shell.h"
#include "string.h"
#include <stdlib.h>


/**
 * consume_comment - consume comment till end of line
 * @src: source string
 * @c: current char
 * @pcurrent: current index
 * @pline: current line number
 */
void consume_comment(const char *src, char c, size_t *pcurrent,
		size_t *pline)
{
	do {
		c = src[(*pcurrent)];
		(*pcurrent)++;
	} while (c != '\0' && c != '\n');
	(*pline)++;
}
/**
 * is_alpha_numeric - check if c is alphabet or numeric char
 * @c: char
 * Return: Trur if c is alpha numeric otherwise false.
 */
bool_t is_alpha_numeric(const char c)
{
	if (IS_DIGIT(c))
		return (TRUE);
	if (c == '|' || c == ';' || c == '&' || c == '>' || c == '<'
			|| c == '\n' || c == ' ' || c == '\t' || c == '\r' || c == '\0'
			|| c == '$')
		return (FALSE);
	return (TRUE);
}
/**
 * consume_word_token - get word token
 * @src: source
 * @c: current char
 * @pcurrent: current index
 * @lst: token list
 * @line: line number
 * @start: start index
 * @pre_token: previous token
 * Return: void
 */
void consume_word_token(const char *src, char c, size_t *pcurrent,
		token_list_t **lst, size_t line, size_t start, token_t **pre_token)
{
	char *tmp = NULL;

	do {
		c = src[(*pcurrent)];
		(*pcurrent)++;
	} while (is_alpha_numeric(c));
	(*pcurrent)--;
	tmp = sub_str(src, start, (*pcurrent) - 1);
	if (tmp != NULL)
	{
		*pre_token = add_token(lst, tmp, line, WORD);
		free(tmp);
	}
}
/**
 * consume_token - consume token
 * @src: src
 * @c: char
 * @pre_token: previous token
 * @lst: token list
 * @pcurrent: current char index
 * @pline: line number
 * Return: void
 */
void consume_token(const char *src, char c, struct token **pre_token,
		token_list_t **lst, size_t *pcurrent, size_t *pline)
{
	char *lex = NULL;
	size_t lo;

	if (c == '\n')
	{
		*pre_token = add_token(lst, "\n", *pline, NEW_LINE);
		(*pline)++;
	}
	else if (c == '|')
		if (match('|', src, pcurrent))
			*pre_token = add_token(lst, "||", *pline, PIPE_PIPE);
		else
			*pre_token = add_token(lst, "|", *pline, PIPE);
	else if (c == ';')
		*pre_token = add_token(lst, ";", *pline, SEMI_COLON);
	else if (c == '&')
		if (match('&', src, pcurrent))
			*pre_token = add_token(lst, "&&", *pline, AMP_AMP);
		else
			*pre_token = add_token(lst, "&", *pline, AMPERSAND);
	else if (c == '>')
		if (match('>', src, pcurrent))
			*pre_token = add_token(lst, ">>", *pline, GREATER_GREATER);
		else
			*pre_token = add_token(lst, ">", *pline, GREATER_THAN);
	else if (c == '<')
		*pre_token = add_token(lst, "<", *pline, LESS_THAN);
	else if (c == '$')
	{
		if (match('$', src, pcurrent))
			*pre_token = add_token(lst, "$$", *pline, DOLLAR_DOLLAR);
		else if (match('?', src, pcurrent))
			*pre_token = add_token(lst, "$?", *pline, DOLLAR_QUESTION);
		else
		{
			lo = *pcurrent - 1;
			while (is_alpha_numeric(src[*pcurrent]))
			(*pcurrent)++;
			lex = sub_str(src, lo, (*pcurrent) - 1);
			if (str_equals(lex, "$") == FALSE)
			*pre_token = add_token(lst, lex, *pline, VARIABLE);
			else
				*pre_token = add_token(lst, lex, *pline, WORD);
			free(lex);
		}
	}
}

