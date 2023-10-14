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
                for (i = len - 1; i >= 0; --i)
                {
                        if (i == 0 && i == '-')
                        {
                                value *= -1;
                        }
                        else
                        {
                                value += (str[i] - 48) * w;
                                w *= 10;
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