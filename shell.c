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
 * write_alias - write alias to file
 * @home_dir - home directory
 * @f_name: file name
 * @head: alias list begin
 * Return: 0 on success
 */
int write_alias(const char *home_dir, const char *f_name, alias_node_t *head)
{
	char *line, *tmp = NULL, *path = NULL;
	int fdout;

	if (IS_NULL_OR_EMPTY(home_dir))
	{
		path = concat_str("./", f_name);
	}
	else
	{
		tmp = concat_str(home_dir, "/");
		path = concat_str(tmp, f_name);
		if (tmp != NULL)
		{
			free(tmp);
			tmp = NULL;
		}
	}
	fdout = open(path, O_WRONLY | O_CREAT | O_TRUNC);
	if (fdout == -1)
	{
		perror("open");
		return (-1);
	}
	while (head != NULL)
	{
		tmp = concat_str(head->data->name, "=");
		line = concat_str(tmp, head->data->value);
		if (tmp != NULL)
		{
			free(tmp);
			tmp = NULL;
		}
		tmp = concat_str(line, "\n");
		_fputs(tmp, fdout);
		if (tmp != NULL)
		{
			free(tmp);
			tmp = NULL;
		}
		if (line != NULL)
		{
			free(line);
			line = NULL;
		}
		head = head->next;
	}
	close(fdout);
	return (0);
}
/**
 * read_alias - read alias file
 * @home_dir: home directory
 * @f_name: file name
 * Return: alias list
 */
alias_node_t *read_alias(const char *home_dir, const char *f_name)
{
	char *tmp = concat_str(home_dir, "/");
	char *path = concat_str(tmp, f_name);
	char *lineptr = NULL;
	char *name = NULL, *value = NULL;
	ssize_t n_read = 0;
	size_t n = 0;
	int index;
	alias_node_t *head = NULL;
	alias_t *a = NULL;
	int fdin;

	if (tmp != NULL)
	{
		free(tmp);
		tmp = NULL;
	}
	fdin = open(path, O_RDONLY | O_CREAT);
	if (path != NULL)
	{
		free(path);
		path = NULL;
	}
	if (fdin == -1)
	{
		perror("open");
		return (NULL);
	}
	while ((n_read = _getline(&lineptr, &n, fdin)) != -1)
	{
		if (!(IS_NULL_OR_EMPTY(lineptr)))
		{
			index = index_of(lineptr, 0, n, '=');
			if (index != -1)
			{
				name = sub_str(lineptr, 0, index - 1);
				value = sub_str(lineptr, index + 1, n - 1);
			}
			else
			{
				name = copy_str(lineptr);
				value = copy_str("");
			}
			a = create_alias(name, value);
			add_to_alias_list(&head, a);
		}
	}
	if (n_read == -1 && errno == 0 && !(IS_NULL_OR_EMPTY(lineptr)))
	{
		index = index_of(lineptr, 0, n, '=');
		if (index != -1)
		{
			name = sub_str(lineptr, 0, index - 1);
			value = sub_str(lineptr, index + 1, n - 1);
		}
		else
		{
			name = copy_str(lineptr);
			value = copy_str("");
		}
		a = create_alias(name, value);
		add_to_alias_list(&head, a);
		free(lineptr);
	}
	close(fdin);
	return (head);
}

