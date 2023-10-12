#include "shell.h"

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

