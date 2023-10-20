#include "session.h"
#include "string.h"
#include "str_list.h"
#include <errno.h>
#include <stdlib.h>
/**
 * create_session - create shell session
 * @argv0: program name
 * @envp: environment variables
 * Return: pointer to newly created session object
 */
sh_session_t *create_session(char *argv0, char **envp)
{
	sh_session_t *session = malloc(sizeof(sh_session_t));
	size_t i = 0, len = 0;

	if (session != NULL)
	{
		session->prompt = copy_str("cisfun$");
		session->sh_name = copy_str(argv0);
		session->alias_file_name = copy_str(".alias");
		len = str_list_len(envp);
		session->env_var_lst = malloc(sizeof(char *) * (len + 1));
		session->env_var_lst[len] = NULL;
		for (i = 0; i < len; ++i)
			session->env_var_lst[i] = copy_str(envp[i]);
		session->status = 0;
	}
	return (session);
}
/**
 * free_session - free memory allocated to session object
 * @session: the session object
 * Return: void
 */
void free_session(sh_session_t **session)
{
	if (session == NULL)
	{
		errno = EINVAL;
		return;
	}
	if (*session != NULL)
	{
		if ((*session)->prompt != NULL)
		{
			free((*session)->prompt);
			(*session)->prompt = NULL;
		}
		if ((*session)->sh_name != NULL)
		{
			free((*session)->sh_name);
			(*session)->sh_name = NULL;
		}
		if ((*session)->alias_file_name != NULL)
		{
			free((*session)->alias_file_name);
			(*session)->alias_file_name = NULL;
		}
		free_str_list(&(*session)->env_var_lst);
		free(*session);
		*session = NULL;
	}
}

