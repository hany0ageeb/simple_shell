#include "shell.h"
#include "string.h"
#include "io.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
size_t str_list_len(char **str_list)
{
    size_t len = 0;
    if (str_list == NULL)
        return (len);
    while (str_list[len] != NULL)
        len++;
    return (len);
}
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
    }
    return (session);
}
void free_session(sh_session_t **session)
{
    size_t i;
    if (session == NULL)
    {
        errno = EINVAL;
        return;
    }
    if (*session != NULL)
    {
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
        if ((*session)->env_var_lst != NULL)
        {
		i = 0;
            while ((*session)->env_var_lst[i] != NULL)
            {
                free((*session)->env_var_lst[i]);
		i++;
            }
            free((*session)->env_var_lst);
            (*session)->env_var_lst = NULL;
            free(*session);
            *session = NULL;
        }
    }
}
bool_t is_regular_file(const char *d, const char *f)
{
    char *path = NULL;
    char *tmp = NULL;
    struct stat st;
    bool_t ret = FALSE;
    if (IS_NULL_OR_EMPTY(d) && IS_NULL_OR_EMPTY(f))
	    return (FALSE);
    else if (IS_NULL_OR_EMPTY(d))
    {
	    path = copy_str(f);
    }
    else if (IS_NULL_OR_EMPTY(f))
    {
	    path = copy_str(d);
    }
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
bool_t file_exists(const char *d, const char *f)
{
    DIR *pDir;
    struct dirent *dir = NULL;
    if (!(IS_NULL_OR_EMPTY(d)) && !(IS_NULL_OR_EMPTY(f)))
    {
        pDir = opendir(d);
        if (pDir == NULL && (errno == EACCES || errno == EBADF))
        {
            perror("open Dir");
            return (FALSE);
        }
        dir = readdir(pDir);
        while (dir != NULL)
        {
            if (str_cmp(dir->d_name, f) == 0)
            {
                if (is_regular_file(d, f) == TRUE)
                {
                    if (closedir(pDir) == -1)
                    {
                        perror("closedir");
                    }
                    return (TRUE);
                }
            }
            dir = readdir(pDir);
        }
        if (dir == NULL && errno == EBADF)
        {
            perror("readdir");
        }
    }
    return (FALSE);
}
void add_to_str_list(char **str_list, const char *value)
{
    size_t len, i = 0;
    char **tmp = NULL;
    if (IS_NULL_OR_EMPTY(value))
    return;
    len = str_list_len(str_list);
    tmp = malloc(sizeof(char *) * (len + 2));
    while (str_list[i] != NULL)
    {
        tmp[i] = copy_str(str_list[i]);
        i++;
    }
    tmp[i] = copy_str(value);
    tmp[i + 1] = NULL;
    free_str_list(str_list);
    str_list = tmp;
}
void remove_from_str_list(char **str_list, const char *start)
{
    size_t i = 0, len, j = 0;
    int exist_index = -1;
    char **tmp;

    if (IS_NULL_OR_EMPTY(start) == TRUE)
    return;
    for (i = 0; str_list[i] != NULL; ++i)
    {
        if (start_with(str_list[i], start) == TRUE)
        {
            exist_index = i;
            break;
        }
    }
    if (exist_index >= 0)
    {
        len = str_list_len(str_list);
        tmp = malloc(sizeof(char *) * (len));
        while (str_list[i] != NULL)
        {
            if (((int)i) != exist_index)
            {
                tmp[j] = str_list[i];
                str_list[i] = NULL;
            }
            else
            {
                free(str_list[i]);
                str_list[i] = NULL;
            }
            i++;
        }
        free(str_list);
        str_list = tmp;
    }
}
void free_str_list(char **str_list)
{
	size_t i;

	if (str_list != NULL && *str_list != NULL)
	{
		for (i = 0; str_list[i] != NULL; ++i)
		{
			free(str_list[i]);
		}
		free(*str_list);
		*str_list = NULL;
	}
}
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
char *_getenv(char *name, char **envp)
{
    char *start_w = NULL;
    size_t i;

    if (envp == NULL)
        return (NULL);
    start_w = concat_str(name, "=");
    for (i = 0; envp[i] != NULL; ++i)
    {
        if (start_with(envp[i], start_w))
        {
            if (start_w != NULL)
                free(start_w);
            return (envp[i]);
        }
    }
    if (start_w != NULL)
        free(start_w);
    return (NULL);
}
int _setenv(const char *name, const char *value, bool_t overwrite, char **envp)
{
    int name_idx = -1;
    size_t i = 0;
    char *tmp;
    char *_name = NULL;

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
/**
 * is_builtin_cmd - check to see if lex is a builtin command
 * @lex: command name
 * Return: True if builtin command otherwise false
*/
bool_t is_builtin_cmd(const char *lex)
{
        if (lex == NULL)
                return (FALSE);
        if (str_equals(lex, "exit") || str_equals (lex, "env"))
                return (TRUE);
        else if (str_equals(lex, "setenv") || str_equals(lex, "unsetenv"))
                return (TRUE);
        else if (str_equals(lex, "cd") || str_equals(lex, "alias"))
            return (TRUE);
        return (FALSE);
}
static void print_cd_error(const char *sh, size_t line, const char *dir)
{
    char *lin = int_to_str(line);
    _puts(sh);
    _puts(": ");
    _puts(lin);
    _puts(": cd: can't cd to ");
    _puts(dir);
    _putc('\n');
    if (lin != NULL)
    {
        free(lin);
        lin = NULL;
    }
}
alias_t *create_alias(char *name, char *value)
{
    alias_t *a = malloc(sizeof(alias_t));
    if (a != NULL)
    {
        a->name = name;
        a->value = value;
    }
    return (a);
}
void free_alias(alias_t **a)
{
    if (a != NULL && *a != NULL)
    {
        if ((*a)->name != NULL)
        {
            free((*a)->name);
            (*a)->name = NULL;
        }
        if ((*a)->value != NULL)
        {
            free((*a)->value);
            (*a)->value = NULL;
        }
        free(*a);
        *a = NULL;
    }
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
void print_alias_all(alias_node_t *head)
{
    while (head != NULL)
    {
        _puts("alias ");
        _puts(head->data->name);
        _puts("=");
        _putc('\'');
        _puts(head->data->value);
        _puts("\'\n");
        head = head->next;
    }
}
void print_alias(const char *name, alias_node_t *head)
{
    while (head != NULL)
    {
        if (str_equals(name, head->data->name))
        {
            _puts("alias ");
        _puts(head->data->name);
        _puts("=");
        _putc('\'');
        _puts(head->data->value);
        _puts("\'\n");
        break;
        }
        head = head->next;
    }
}
alias_node_t *add_to_alias_list(alias_node_t **head, alias_t *a)
{
    alias_node_t *node = NULL, *end = NULL;
    if (a == NULL)
    return (NULL);
    node = malloc(sizeof(alias_node_t));
    if (node != NULL)
    {
        node->data = a;
        node->next = NULL;
        if (*head == NULL)
        {
            *head = node;
        }
        else
        {
            end = *head;
            while (end->next != NULL)
            {
                end = end->next;
            }
            end->next = node;
        }
    }
    return (node);
}
alias_node_t *add_or_update_alias_list(alias_node_t **head,char *name, char *value)
{
    alias_node_t *v;
    if (IS_NULL_OR_EMPTY(name))
    return (NULL);
    if (*head == NULL)
    {
        return (add_to_alias_list(head, create_alias(name, value)));
    }
    else
    {
        v = *head;
        do {
            if (str_equals(v->data->name, name))
            {
                if (v->data->value != NULL)
                free(v->data->value);
                v->data->value = value;
                return (v);
            }
        } while(v != NULL);
        return (add_to_alias_list(head, create_alias(name, value)));
    }
}
int alias_exec(simple_command_t *command, sh_session_t *session)
{
    char *home_dir = NULL;
    alias_node_t *head = NULL;
    token_node_t *v = NULL;
    char *name, *value;
    size_t len;
    int index;

    home_dir = _getenv("HOME", session->env_var_lst);
    if (home_dir != NULL)
    {
        head = read_alias(home_dir, session->alias_file_name);
    }
    if (command->args == NULL || command->args->head == NULL)
    {
        print_alias_all(head);
    }
    else
    {
        do {
            len = str_len(v->token->lexeme);
            if ((index = index_of(v->token->lexeme, 0, len - 1,'=')) >= 0)
            {
                name = sub_str(v->token->lexeme, 0, index - 1);
                value = sub_str(v->token->lexeme, index + 1, len - 1);
                add_or_update_alias_list(&head, name, value);
            }
            else
            {
                print_alias(v->token->lexeme, head);
            }
        } while (v != NULL);
    }
    if (head != NULL)
    {
        write_alias(home_dir, session->alias_file_name, head);
    }
    return (0);
}
int setenv_exec(simple_command_t *command, sh_session_t *session)
{
    size_t len = 0;
    token_node_t *node;
    char *lin;
    if (command->args == NULL || command->args->head == NULL)
    {
        return (env_exec(command, session));
    }
    node = command->args->head;
    while (node != NULL)
    {
        len++;
        node = node->next;
    }
    if (len > 2)
    {
        _puts(session->sh_name);
        _puts(": ");
        lin = int_to_str(command->cmd->line);
        _puts(lin);
        _puts(": setenv: too many arguments");
        _putc('\n');
        return (1);
    }
    return (_setenv(command->args->head->token->lexeme, command->args->head->next->token->lexeme, TRUE, session->env_var_lst));
}
int unsetenv_exec(simple_command_t *command, sh_session_t *session)
{
    char *lin;
    token_node_t *node;

    if (command->args == NULL || command->args->head == NULL)
    {
        _puts(session->sh_name);
        _puts(": ");
        lin = int_to_str(command->cmd->line);
        _puts(lin);
        _puts(": unsetenv: too few arguments");
        _putc('\n');
        return (1);
    }
    node = command->args->head;
    while (node != NULL)
    {
        _unsetenv(node->token->lexeme, session->env_var_lst);
        node = node->next;
    }
    return (0);
}
int cd_exec(simple_command_t *command, sh_session_t *session)
{
    char *home = NULL;
    char *pwd = NULL;
    char *oldpwd = NULL;
    DIR* dir = NULL;

    if (command->args == NULL || command->args->head == NULL)
    {
        /*cd $HOME*/
        home = _getenv("HOME", session->env_var_lst);
        if (home != NULL)
        {
            chdir(home);
            pwd = _getenv("PWD", session->env_var_lst);
            if (pwd != NULL)
            _setenv("OLDPWD", pwd, TRUE, session->env_var_lst);
            _setenv("PWD", home, TRUE, session->env_var_lst);
            free(home);
        }
    }
    else
    {
        if (str_cmp(command->args->head->token->lexeme, "-") == 0)
        {
            oldpwd = _getenv("OLDPWD", session->env_var_lst);
            pwd = _getenv("PWD", session->env_var_lst);
            if (oldpwd != NULL)
            {
                chdir(oldpwd);
                _setenv("PWD", oldpwd, TRUE, session->env_var_lst);
                if (pwd != NULL)
                {
                    _setenv("OLDPWD", pwd, TRUE, session->env_var_lst);
                }
            }
        }
        else
        {
            dir = opendir(command->args->head->token->lexeme);
            if (dir == NULL)
            {
                print_cd_error(session->sh_name, command->cmd->line, command->args->head->token->lexeme);
                return(2);
            }
            else
            {
                closedir(dir);
                dir = NULL;
                oldpwd = _getenv("PWD", session->env_var_lst);
                _setenv("PWD", command->args->head->token->lexeme, TRUE, session->env_var_lst);
                if (oldpwd != NULL)
                {
                    _setenv("OLDPWD", oldpwd, TRUE, session->env_var_lst);
                    free(oldpwd);
                }
            }
        }
    }
    return (0);
}
int env_exec(simple_command_t *command, sh_session_t *session)
{
	size_t i;

	if (command == NULL || command->cmd == NULL || !str_equals(command->cmd->lexeme, "env"))
		return (-1);
        for (i = 0; session->env_var_lst[i] != NULL; ++i)
        {
            _puts(session->env_var_lst[i]);
            _putc('\n');
        }
        return (0);
}
int exit_exec(simple_command_t *command, sh_session_t *session)
{
        int exit_code = 0;
        if (command->args != NULL && command->args->head != NULL)
        {
                exit_code = str_to_int(command->args->head->token->lexeme);
                if (exit_code == 0 && errno == EINVAL)
                {
                        char *message = concat_str(session->sh_name, int_to_str(command->cmd->line));
                        _puts(message);
                        session->status = 2;
                        return (2);
                }
        }
        session->status = exit_code;
        exit(exit_code);
}

