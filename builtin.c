#include "string.h"
#include "builtin.h"
#include "env.h"
#include <dirent.h>
#include <errno.h>
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
    char *home = NULL, *pwd = NULL, *oldpwd = NULL;
    DIR* dir = NULL;

    if (command->args == NULL || command->args->head == NULL)
    {
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

