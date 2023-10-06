#ifndef TOKEN_H_
#define TOKEN_H_
#include <stddef.h>
/**
 * enum token_type - token type
 * @SEMI_COLON: semi colon token ;
 * @PIPE: PIPE token |
 * @GREATER_THAN: greater than token >
 * @LESS_THAN: leass than token <
 * @PIPE_PIPE: || token type
 * @AMPERSAND: & token type
 * @AMP_AMP: && token type
*/
enum token_type
{
    SEMI_COLON = 1,
    PIPE = 2,
    PIPE_PIPE = 3,
    AMPERSAND = 4,
    AMP_AMP = 5,
    NEW_LINE = 6,
    WORD = 7,
    NUMBER = 8,
    GREATER_THAN = 9,
    LESS_THAN = 10,
    GREATER_GREATER = 11
};
/**
 * struct token - token
*/
struct token
{
    char *lexeme;
    size_t line;
    enum token_type type;
};
struct token_node
{
    struct token *token;
    struct token_node *next;
};
struct token_list
{
    struct token_node *head;
};
struct token *create_token(const char *lexeme, size_t line, enum token_type type);
void free_token(struct token **tok);
struct token_node *create_token_node(struct token *token);
void free_token_node(struct token_node **node);
struct token_list *create_token_list();
void add_token_to_list(struct token_list *lst, struct token *token);
void free_token_list();
#endif
