#ifndef SESSION_H_
#define SESSION_H_
/**
 * struct sh_session - shell session
 * @env_var_lst: environ
 * @sh_name: program name
 * @prompt: shell prompt
 * @status: last command exit code!
 * @alias_file_name: alias file full path
 */
typedef struct sh_session
{
	char **env_var_lst;
	char *sh_name;
	char *prompt;
	int status;
	char *alias_file_name;
} sh_session_t;
sh_session_t *create_session(char *argv0, char **envp);
void free_session(sh_session_t **session);
#endif
