#ifndef SHELL_H_
#define SHELL_H_
#include "token.h"
#include "bool.h"
#include "simple_command.h"
#include "session.h"
#include "alias_list.h"
#include <sys/types.h>

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
void replace_variables(token_list_t *args, sh_session_t *session);
int parse_tokens(const token_list_t *lst, sh_session_t *session);
char **get_paths(char **envp);
char *find_full_path(char *cmd, char **paths);
int write_alias(const char *home_dir, const char *f_name, alias_node_t *head);
alias_node_t *read_alias(const char *home_dir, const char *f_name);
#endif

