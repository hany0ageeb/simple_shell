#include "shell.h"
#include "string.h"
#include "io.h"
#include <errno.h>
/**
 * is_builtin_cmd - check to see if lex is a builtin command
 * @lex: command name
 * Return: True if builtin command otherwise false
*/
bool_t is_builtin_cmd(const char *lex)
{
        if (lex == NULL)
                return (FALSE);
        if (str_cmp(lex, "exit") == 0 || str_cmp (lex, "env") == 0)
                return (TRUE);
        else if (str_cmp(lex, "setenv") == 0 || str_cmp(lex, "unsetenv") == 0)
                return (TRUE);
        return (FALSE);
}
int env_exec(simple_command_t *command, sh_session_t *session)
{
        size_t i;

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