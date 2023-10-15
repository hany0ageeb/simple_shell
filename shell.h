#ifndef SHELL_H_
#define SHELL_H_
#include "token.h"
#include "bool.h"
#include "simple_command.h"
typedef struct sh_session
{
	char **env_var_lst;
	char *sh_name;
	char *prompt;
	int status;
} sh_session_t;
typedef struct builtin_cmd
{
	char *cmd;
	int (*execute)(simple_command_t *command, sh_session_t *session);
};
typedef struct alias
{
	char *name;
	char *value;
} alias_t;
sh_session_t *create_sesssion(const char *argv0, const char **envp);
void free_session(sh_session_t **session);
void report_error(const char *argv0, token_t *pre_token,
		const char *lexeme, size_t line, const char c, const char next_char);
bool_t is_valid_token(const token_t *pre_token, const char c);
token_t *add_token(token_list_t **lst, const char *lexeme, size_t line,
		token_type_t type);
bool_t match(char expected, const char *src, size_t *pcurrent);
void consume_comment(const char *src, char c, size_t *pcurrent, size_t *pline);
bool_t is_alpha_numeric(const char c);
void consume_word_token(const char *src, char c, size_t *pcurrent,
		token_list_t **lst, size_t line, size_t start, token_t **pre_token);
void consume_token(const char *src, char c, token_t **pre_token,
		token_list_t **lst, size_t *pcurrent, size_t *pline);
token_node_t *get_op(token_node_t *start, token_node_t *end);
bool_t scan_tokens(const char *src, token_list_t **lst, const char *argv0);
simple_command_t *get_simple_command(token_node_t *start, token_node_t *end);
int parse_tokens(const token_list_t *lst);
bool_t is_builtin_cmd(const char *lex);
char **get_paths(const char **envp);
char *find_full_path(const char *cmd, const char **paths);
#endif

