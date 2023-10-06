#include "token.h"
#include "string.h"
#include <stdlib.h>
#include <errno.h>
struct token *create_token(const char *lexeme, size_t line, enum token_type type)
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
    }
    errno = 0;
}