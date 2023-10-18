#include "env.h"
#include "string.h"
#include "str_list.h"
#include <stdlib.h>
#include <errno.h>

/**
 * _getenv - mimic getenv function in stdlib.h
 * @name: env variable name
 * @envp: env variables
 * Return: its value or NULL
 */
char *_getenv(char *name, char **envp)
{
	char *start_w = NULL;
	char *var_value = NULL;
	size_t i, len = 0;
	int index = -1;

	if (envp == NULL)
		return (NULL);
	start_w = concat_str(name, "=");
	for (i = 0; envp[i] != NULL; ++i)
	{
		if (start_with(envp[i], start_w))
		{
			len = str_len(envp[i]);
			index = index_of(envp[i], 0, len - 1, '=');
			if (index >= 0)
			{
				var_value = sub_str(envp[i], index, len - 1);
			}
		}
	}
	if (start_w != NULL)
		free(start_w);
	return (var_value);
}
/**
 * _setenv - setenv
 * @name: env name
 * @value: env value
 * @overwrite: overwrite
 * @envp: envp
 * Return: 0 on success
 */
int _setenv(const char *name, const char *value, bool_t overwrite, char **envp)
{
	int name_idx = -1;
	size_t i = 0;
	char *tmp, *_name = NULL;

	if (IS_NULL_OR_EMPTY(name) == TRUE || contains_char(name, '=') == TRUE)
	{
		errno = EINVAL;
		return (-1);
	}
	while (envp[i] != NULL)
	{
		if (start_with(envp[i], name) == TRUE)
		{
			name_idx = i;
			break;
		}
		i++;
	}
	if (name_idx == -1)
	{
		_name = concat_str(name, "=");
		add_to_str_list(envp, _name);
		free(_name);
	}
	else if (name_idx >= 0 && overwrite == TRUE)
	{
		free(envp[i]);
		tmp = concat_str(name, "=");
		envp[i] = concat_str(tmp, value);
		if (tmp != NULL)
		{
			free(tmp);
			tmp = NULL;
		}
	}
	return (0);
}
/**
 * _unsetenv - unsetenv
 * @name: env name
 * @envp: envp
 * Return: 0 on success
 */
int _unsetenv(const char *name, char **envp)
{
	char *start_with = NULL;

	if (IS_NULL_OR_EMPTY(name) || contains_char(name, '=') == TRUE)
	{
		errno = EINVAL;
		return (-1);
	}
	start_with = concat_str(name, "=");
	remove_from_str_list(envp, start_with);
	if (start_with != NULL)
	{
		free(start_with);
		start_with = NULL;
	}
	return (0);
}

