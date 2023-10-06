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
    SEMI_COLON,
    PIPE,
    PIPE_PIPE,
    AMPERSAND,
    AMP_AMP,
    NEW_LINE,
    WORD,
    NUMBER,
    GREATER_THAN,
    LESS_THAN,
    GREATER_GREATER
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