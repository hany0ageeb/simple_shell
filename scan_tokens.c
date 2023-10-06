#include "shell.h"
#include "token.h"
#include "bool.h"
#include "string.h"
#include <stdlib.h>
inline static char advance(const char *src, size_t *pcurrent)
{
    return (src[(*pcurrent)++]);
}
inline static enum boolean is_alpha_numeric(const char c)
{
    if (IS_DIGIT(c))
    return (TRUE);
    if (c != ';' && c != '&' && c != '|' && c != '>' && c != '<')
    return (TRUE);
    return (FALSE);
}
inline static enum boolean is_number(const char *str)
{
    size_t i;

    if (IS_NULL_OR_EMPTY(str))
    return (FALSE);
    for (i = 0; str[i] != '\0'; ++i)
    {
        if (!IS_DIGIT(str[i]))
        return (FALSE);
    }
    return (TRUE);
}
inline static enum boolean is_word(const char *str)
{
    size_t i;
    if (IS_NULL_OR_EMPTY(str))
    return (FALSE);
    for (i = 0; str[i] != '\0'; ++i)
    {
        if (IS_DIGIT(str[i]))
        return (FALSE);
    }
    return (TRUE);
}
static enum boolean match(char expected, const char *src, size_t *pcurrent)
{
    if (src[*pcurrent] == '\0')
    return (FALSE);
    if (src[*pcurrent] != expected)
    return (FALSE);
    (*pcurrent)++;
    return (TRUE);
}
static void add_token(struct token_list **lst, const char *lexeme, size_t line, enum token_type type)
{
    struct token *token = NULL;

    if (*lst == NULL)
        *lst = create_token_list();
    token = create_token(lexeme, line, type);
    if (token != NULL)
        add_token_to_list(*lst, token);
}
struct token_list *scan_tokens(const char *src)
{
    size_t start = 0, current = 0, line = 1;
    char c;
    struct token_list *lst = NULL;
    char *tmp;

    while (src[current] != '\0')
    {
        c = advance(src, &current);
        switch (c)
        {
            case '\t':
            case ' ':
            case '\r':
            break;
            case ';':
            add_token(&lst, ";", line, SEMI_COLON);
            break;
            case '|':
            if (match('|', src, &current))
            add_token(&lst, "||", line, PIPE_PIPE);
            else
            add_token(&lst, "|", line, PIPE);
            break;
            case '&':
            if (match('&', src, &current))
            add_token(&lst, "&&", line, AMP_AMP);
            else
            add_token(&lst, "&", line, AMPERSAND);
            break;
            case '>':
            if (match('>', src, &current))
            add_token(&lst, ">>", line, GREATER_GREATER);
            else
            add_token(&lst, ">", line, GREATER_THAN);
            break;
            case '<':
            add_token(&lst, "<", line, LESS_THAN);
            break;
            case '\n':
            add_token(&lst, "\n", line, NEW_LINE);
            line++;
            break;
            case '#':
            while (src[current] != '\0' && src[current] != '\n')
            advance(src, &current);
            break;
            default:
            while (is_alpha_numeric(src[current]) && src[current] != '\0' && src[current] != '\n')
            advance(src, &current);
            tmp = sub_str(src, start, current - 1);
            if (tmp != NULL)
            {
                if (is_word(tmp))
                add_token(&lst, tmp, line, WORD);
                else if (is_number(tmp))
                add_token(&lst, tmp, line, NUMBER);
                free(tmp);
            }
            break;
        }
    }
    return (lst);
}