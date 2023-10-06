#include "bool.h"
#include "shell.h"
#include "token.h"
#include <stdio.h>
#include "io.h"
#include <unistd.h>
#include <stdlib.h>
static int run(const char *src);
static int run_prompt();
static int run_file(const char *file);
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        return (run_prompt());
    }
    else if (argc == 2)
    {
        return (run_file(argv[1]));
    }
    else
    {
        /*Usage Error*/
        exit(EXIT_FAILURE);
    }
}
static void print_tok_lst(struct token_list *lst)
{
    struct token_node *node = NULL;

    if (lst != NULL && lst->head != NULL)
    {
        node = lst->head;
        do {
            _puts(node->token->lexeme);
            _puts("\t\n");
            node = node->next;
        } while (node != NULL);
    }
}
static int run(const char *src)
{
    struct token_list *tok_lst = NULL;
    tok_lst = scan_tokens(src);
    if (tok_lst != NULL)
    {
        print_tok_lst(tok_lst);
        free_token_list(&tok_lst);
    }
    return (0);
}
static int run_prompt()
{
    const char *prompt = "#cisfun$";
    char *line;
    size_t n = 0;
    ssize_t n_read, run_result;
    while (TRUE)
    {
        _puts(prompt);
        n_read = getline(&line, &n, stdin);
        if (n_read == -1)
        return (-1);
        run_result = run(line);
        free(line);
        if (run_result == -1)
        return (-1);
    }
}
static int run_file(const char *file)
{
    _puts(file);
    return (0);
}
