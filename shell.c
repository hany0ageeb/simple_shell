#include "shell.h"
#include "string.h"
#include "io.h"
#include "env.h"
#include "alias_list.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

/**
 * find_full_path - find full path
 * @cmd: command string
 * @paths: search paths
 * Return: path if found or null
 */
char *find_full_path(char *cmd, char **paths)
{
	size_t i = 0;
	char *full_path = NULL;
	char *tmp = NULL;

	if (IS_NULL_OR_EMPTY(cmd))
		return (NULL);
	if (paths == NULL || *paths == NULL)
		return (NULL);
	while (paths[i] != NULL)
	{
		if (file_exists(paths[i], cmd))
		{
			tmp = concat_str(paths[i], "/");
			full_path = concat_str(tmp, cmd);
			if (tmp != NULL)
			{
				free(tmp);
				tmp = NULL;
			}
			return (full_path);
		}
		i++;
	}
	return (full_path);
}
/**
 * get_paths - get paths
 * @envp: envp
 * Return: paths
 */
char **get_paths(char **envp)
{
	char *path = NULL;
	char **paths = NULL;

	if (envp == NULL)
		return (NULL);
	path = _getenv("PATH", envp);
	if (path != NULL)
	{
		paths = split_str(path, ':');
	}
	if (path != NULL)
	{
		free(path);
		path = NULL;
	}
	return (paths);
}
/**
 * unload_alias - unload alias into file
 * @session: session
 * Return: void
 */
void unload_alias(sh_session_t *session)
{
	alias_node_t *v = NULL;
	char *alias = NULL;
	int fd = open(session->alias_file_name, O_WRONLY | O_CREAT | O_TRUNC);

	if (fd == -1)
	{
		perror("unload alias");
		return;
	}
	if (session != NULL)
	{
		v = session->alias_list;
		while (v != NULL)
		{
			alias = concat_strs(3, v->data->name, "=", v->data->value, "\n");
			_fputs(alias, fd);
			if (alias != NULL)
			{
				free(alias);
				alias = NULL;
			}
			v = v->next;
		}
	}
	close(fd);
}
/**
 * read_alias_file - read alias from file
 * @fd: file desc
 * @session: session
 * Return: void
 */
static void read_alias_file(int fd, sh_session_t *session)
{
	char *name, *value, *lineptr = NULL;
	size_t n = 0;
	ssize_t n_read = 0, index;

	if (fd == -1 || session == NULL)
	{
		errno = EINVAL;
		return;
	}
	while ((n_read = _getline(&lineptr, &n, fd)) != -1)
	{
		if (n_read > 0 && lineptr[0] != '\n')
		{
			index = index_of(lineptr, 0, n_read - 1, '=');
			if (index != -1)
			{
				name = sub_str(lineptr, 0, index - 1);
				if (lineptr[n_read - 1] == '\n')
					value = sub_str(lineptr, index + 1, n_read - 2);
				else
					value = sub_str(lineptr, index + 1, n_read - 1);
			}
			else
			{
				name = copy_str(lineptr);
				value = copy_str("");
			}
			add_to_alias_list(&session->alias_list, create_alias(name, value));
		}
	}
	if (lineptr != NULL)
		free(lineptr);
}
/**
 * load_alias - load alias from file
 * @session: session
 * Return: void
 */
void load_alias(sh_session_t *session)
{
	int fd;

	fd = open(session->alias_file_name, O_RDONLY | O_CREAT);
	if (fd == -1)
		return;
	read_alias_file(fd, session);
	close(fd);
}

