#include "io.h"
#include "mem.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
/**
 * _getline - getline
 * @lineptr: line pointer
 * @n: ptr size
 * @fdin: file descriptror
 * Return: -1 or number or read chars
 */
ssize_t _getline(char **lineptr, size_t *n, int fdin)
{
	const size_t INITALLOC = 16, ALLOCSTEP = 16;
	size_t num_read = 0, new_size = 0;
	char *tmp = NULL;
	int c = -1;

	if ((lineptr == NULL) || (n == NULL))
	{
		errno = EINVAL;
		return (-1);
	}
	if (*lineptr == NULL)
	{
		*lineptr = malloc(sizeof(char) * (INITALLOC + 1));
		if (*lineptr == NULL)
			return (-1);
		*n = INITALLOC;
	}
	while ((c = _fgetc(fdin)) != -1 && c != '\n')
	{
		num_read++;
		if (num_read >= *n)
		{
			new_size = *n + ALLOCSTEP;
			tmp = _realloc(*lineptr, *n + 1, new_size + 1);
			if (tmp == NULL)
				return (-1);
			*lineptr = tmp;
			*n = new_size;
		}
		(*lineptr)[num_read - 1] = (char)c;
	}
	if (c == '\n')
		(*lineptr)[num_read++] = '\n';
	(*lineptr)[num_read] = '\0';
	if (c == -1)
	{
		errno = 0;
		return (-1);
	}
	return ((ssize_t)num_read);
}

/**
 * file_exists - file exists
 * @d: director
 * @f: file
 * Return: TRUE if exists otherwise FALSE
 */
bool_t file_exists(const char *d, const char *f)
{
	char *path = NULL;
	char *tmp;
	bool_t ret = FALSE;

	if (IS_NULL_OR_EMPTY(d) && IS_NULL_OR_EMPTY(d))
		return (FALSE);
	if (!(IS_NULL_OR_EMPTY(d)))
	{
		path = copy_str(d);
	}
	if (!(IS_NULL_OR_EMPTY(f)))
	{
		if (path != NULL)
		{
			tmp = concat_str(path, "/");
			free(path);
			path = concat_str(tmp, f);
			free(tmp);
		}
		else
		{
			path = copy_str(f);
		}
	}
	if (path != NULL)
	{
		if (access(path, X_OK) == 0)
			ret = TRUE;
		else
			ret = FALSE;
		free(path);
	}
	return (ret);
}
/**
 * dir_exists - concat path with dir adding / and check for existance
 * @path: from cd paths
 * @dir: dir name
 * Return: TRUE if path/dir/ do exists otherwise FALSE
 */
char *dir_exists(const char *path, const char *dir)
{
	char *_path = NULL;
	char *_dir = NULL;
	char *full_path = NULL;
	bool_t ret = FALSE;

	if (IS_NULL_OR_EMPTY(path) && IS_NULL_OR_EMPTY(dir))
		return (NULL);
	if (IS_NULL_OR_EMPTY(path) && start_with(dir, "/") == FALSE)
		_path = copy_str("./");
	else if (IS_NULL_OR_EMPTY(path))
		_path = copy_str(".");
	else if (end_with_char(path, '/') == FALSE && start_with(dir, "/") == FALSE)
		_path = concat_str(path, "/");
	else
		_path = copy_str(path);
	if (end_with_char(dir, '/') == FALSE)
		_dir = concat_str(dir, "/");
	else if (!(IS_NULL_OR_EMPTY(dir)))
		_dir = copy_str(dir);
	full_path = concat_str(_path, _dir);
	if (access(full_path, F_OK) == 0)
		ret = TRUE;
	else
		ret = FALSE;
	if (_path != NULL)
	{
		free(_path);
		_path = NULL;
	}
	if (_dir != NULL)
	{
		free(_dir);
		_dir = NULL;
	}
	if (full_path != NULL && ret == FALSE)
	{
		free(full_path);
		full_path = NULL;
	}
	return (full_path);
}
/**
 * is_regular_file - is regular file
 * @d: directory name
 * @f: file name
 * Return: TRUE if it is a r file
 */
bool_t is_regular_file(const char *d, const char *f)
{
	char *path = NULL;
	char *tmp = NULL;
	struct stat st;
	bool_t ret = FALSE;

	if (IS_NULL_OR_EMPTY(d) && IS_NULL_OR_EMPTY(f))
		return (FALSE);
	else if (IS_NULL_OR_EMPTY(d))
		path = copy_str(f);
	else if (IS_NULL_OR_EMPTY(f))
		path = copy_str(d);
	else
	{
	    tmp = concat_str(d, "/");
	    path = concat_str(tmp, f);
	}
	if (tmp != NULL)
	{
		free(tmp);
		tmp = NULL;
	}
	if (stat(path, &st) == 0)
	{
		if ((st.st_mode & S_IFMT) == S_IFREG)
			ret = TRUE;
		else
			ret = FALSE;
	}
	if (path != NULL)
	{
		free(path);
		path = NULL;
	}
	return (ret);
}

