#include "io.h"
#include "mem.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
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
	const size_t INITALLOC = 16;
	const size_t ALLOCSTEP = 16;
	size_t num_read = 0;
	size_t new_size;
	char *tmp;
	int c;

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
	DIR *pDir;
	struct dirent *dir = NULL;

	if (!(IS_NULL_OR_EMPTY(d)) && !(IS_NULL_OR_EMPTY(f)))
	{
		pDir = opendir(d);
		if (pDir == NULL)
			return (FALSE);
		dir = readdir(pDir);
		while (dir != NULL)
		{
			if (str_cmp(dir->d_name, f) == 0)
			{
				if (is_regular_file(d, f) == TRUE)
				{
					closedir(pDir);
					return (TRUE);
				}
			}
			dir = readdir(pDir);
		}
		closedir(pDir);
	}
	return (FALSE);
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

