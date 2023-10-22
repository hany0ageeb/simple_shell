#include "builtin.h"
#include "env.h"
#include "io.h"
#include <stdlib.h>
/**
 * _cd_oldpwd - cd to oldpwd
 * @session: shell session
 * @pwd: $PWD
 * Return: 0 on success 1 on failure
 */
int _cd_oldpwd(sh_session_t *session, const char *pwd)
{
	char *oldpwd = NULL;
	int ret = 0;

	oldpwd = _getenv("OLDPWD", session->env_var_lst);
	if (oldpwd == NULL)
		ret = 1;
	else if (oldpwd[0] == '\0')
		ret = 0;
	else
		ret = _cd(oldpwd, session, pwd);
	_puts(oldpwd != NULL ? oldpwd : "\n");
	if (oldpwd != NULL)
	{
		free(oldpwd);
		oldpwd = NULL;
	}
	return (ret);
}
/**
 * _cd_home - cd to $HOME
 * @session: shell session
 * @pwd: $PWD
 * Return: 0 on success
 */
int _cd_home(sh_session_t *session, const char *pwd)
{
	char *home = NULL;
	int ret;

	home = _getenv("HOME", session->env_var_lst);
	if (home == NULL)
		ret = 1;
	else if (home[0] == '\0')
		ret = 0;
	else
		ret = _cd(home, session, pwd);
	if (home != NULL)
	{
		free(home);
		home = NULL;
	}
	return (ret);
}

