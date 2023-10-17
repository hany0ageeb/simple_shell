#ifndef SESSION_H_
#define SESSION_H_
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
