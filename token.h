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
 * @NEW_LINE: new line token
 * @WORD: word token
 * @NUMBER: number token (not used)
 * @GREATER_GREATER: >> token type
*/
typedef enum token_type
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
} token_type_t;
/**
 * struct token - token
 * @lexeme: lexeme
 * @line: line number
 * @type: token type
*/
typedef struct token
{
	char *lexeme;
	size_t line;
	enum token_type type;
} token_t;
/**
 * struct token_node - token node structure
 * @token: token (data of the node)
 * @next: pointer to next node
 */
typedef struct token_node
{
	struct token *token;
	struct token_node *next;
} token_node_t;
/**
 * struct token_list - list of tokens
 * @head: head node
 */
typedef struct token_list
{
	struct token_node *head;
} token_list_t;
struct token *create_token(const char *lexeme, size_t line, token_type_t type);
struct token *copy_token(const struct token *src);
void free_token(struct token **tok);
struct token_node *create_token_node(struct token *token);
token_node_t *copy_token_node(const token_node_t *src);
void free_token_node(struct token_node **node);
struct token_list *create_token_list(void);
token_list_t *copy_token_list(const token_node_t *head, const token_node_t *end);
void add_token_to_list(struct token_list *lst, struct token *token);
void free_token_list(struct token_list **lst);
#endif

