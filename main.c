#include "bool.h"
#include "shell.h"
#include "token.h"
#include <stdio.h>
#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
static int run(const char *src, sh_session_t *session);
static int run_prompt(sh_session_t *session);
static int run_file(const char *file, sh_session_t *session);
int main(int argc, char **argv, char **envp)
{
    sh_session_t *session = create_sesssion(argv[0], envp);
    int ret;

    if (session == NULL)
    {
        perror("Create session");
        return (-1);
    }
    if (argc == 1)
    {
        ret = run_prompt(session);
    }
    else if (argc == 2)
    {
        ret = run_file(argv[1], session);
    }
    else
    {
        /*TODO: */
        ret = EXIT_FAILURE;
    }
    if (session != NULL)
    {
        free_session(&session);
    }
    return (ret);
}
static int run(const char *src, sh_session_t *session)
{
    token_list_t *tok_lst = NULL;
    if (scan_tokens(src, &tok_lst, session->sh_name))
    {
        parse_tokens(tok_lst);
    }
    else
    {
        return (-1);
    }
    return (0);
}
static int run_prompt(sh_session_t *session)
{
    const char *prompt = "#cisfun$";
    char *line;
    size_t n = 0;
    ssize_t n_read, run_result;

    while (TRUE)
    {
        _puts(session->prompt);
        n_read = getline(&line, &n, stdin);
        if (n_read == -1)
        {
            return (-1);
        }
        run_result = run(line, session);
        free(line);
    }
    return (run_result);
}
static int run_file(const char *file, sh_session_t *session)
{
    int fd = open(file, O_RDONLY);
    char *line = NULL;
    size_t n = 0;
    ssize_t n_read;
    if (fd == -1)
    {
        perror("Open File");
        return (-1);
    }
    while ((n_read = getline(&line, &n, fd)) != -1)
    {
        
    }
    if (line != NULL)
        free(line);
    return (0);
}
