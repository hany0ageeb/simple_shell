#ifndef SHELL_H_
#define SHELL_H_
#include "token.h"
#include "bool.h"
#include "simple_command.h"
#include <sys/types.h>
typedef struct sh_session
{
	char **env_var_lst;
	char *sh_name;
	char *prompt;
	int status;
	char *alias_file_name;
} sh_session_t;
typedef struct alias
{
	char *name;
	char *value;
} alias_t;
typedef struct alias_node
{
	struct alias* data;
	struct alias_node *next;
} alias_node_t;
alias_t *create_alias(char *name, char *value);
void free_alias(alias_t **a);
alias_node_t *add_to_alias_list(alias_node_t **head, alias_t *al);
alias_node_t *add_or_update_alias_list(alias_node_t **head, char *name, char *value);
void free_alias_list(alias_node_t **head);
sh_session_t *create_session(char *argv0, char **envp);
void free_session(sh_session_t **session);
size_t str_list_len(char **str_list);
void free_str_list(char **str_list);
void add_to_str_list(char **str_list, const char *value);
void remove_from_str_list(char **str_list, const char *start_with);
bool_t is_regular_file(const char *d, const char *f);
bool_t file_exists(const char *d, const char *f);
bool_t is_valid_num(const char *str);
int int_len(ssize_t num);
char *int_to_str(ssize_t num);
int str_to_int(const char *str);
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
simple_command_t *get_simple_command(token_node_t *start, token_node_t *end, sh_session_t *session);
int parse_tokens(const token_list_t *lst, sh_session_t *session);
bool_t is_builtin_cmd(const char *lex);
char **get_paths(char **envp);
char *find_full_path(char *cmd, char **paths);
char *_getenv(char *name, char **envp);
int cd_exec(simple_command_t *command, sh_session_t *session);
int env_exec(simple_command_t *command, sh_session_t *session);
int exit_exec(simple_command_t *command, sh_session_t *session);
int alias_exec(simple_command_t *command, sh_session_t *session);
int setenv_exec(simple_command_t *command, sh_session_t *session);
int unsetenv_exec(simple_command_t *command, sh_session_t *session);
#endif

