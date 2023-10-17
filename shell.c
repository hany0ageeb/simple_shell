#include "shell.h"
#include "string.h"
#include "io.h"
#include "alias.h"
#include "alias_list.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

char *find_full_path(char *cmd, char **paths)
{
    size_t i;
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
bool_t is_valid_num(const char *str)
{
        int i;
        if (IS_NULL_OR_EMPTY(str))
        return (FALSE);
        if (str[0] == '-' || str[0] == '+')
        i = 1;
        else
        i = 0;
        for (; str[i] != '\0'; ++i)
        {
                if (!(IS_DIGIT(str[i])))
                {
                        return (FALSE);
                }
        }
        return (TRUE);
}
int int_len(ssize_t num)
{
        int len = 0;
        if (num < 0)
        {
                len++;
                num *= -1;
        }
        do
        {
                len++;
                num /= 10;
        } while (num > 0);
        return (len);
}
char *int_to_str(ssize_t num)
{
        const char *digits = "0123456789";
        int len = int_len(num);
        int i = len - 1;
        char *str = malloc(sizeof(char) * (len + 1));
        if (str != NULL)
        {
                if (num < 0)
                {
                        str[0] = '-';
                        num *= -1;
                }
                do
                {
                        str[i] = digits[num % 10];
                        num /= 10;
                        i--;
                } while (num > 0);
        }
        return (str);
}
int str_to_int(const char *str)
{
        const int BASE = 10;
        int value = 0, w = 1, i;
        size_t len;
        if (is_valid_num(str))
        {
		len = str_len(str);
                for (i = len - 1; i >= 0; --i)
                {
                        if (i == 0 && i == '-')
                        {
                                value *= -1;
                        }
                        else
                        {
                                value += (str[i] - 48) * w;
                                w *= BASE;
                        }
                }
                errno = 0;
                return (value);
        }
        else
        {
                errno = EINVAL;
                return (0);
        }
}
char **get_paths(char **envp)
{
	char *path = NULL;
    if (envp == NULL)
        return (NULL);
    path = _getenv("PATH", envp);
    if (path != NULL)
    {
        return (split_str(path, ':'));
    }
    return (NULL);
}


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
        if(line != NULL)
        {
        free(line);
        line = NULL;
        }
        head = head->next;
    }
    close(fdout);
    return (0);
}
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


