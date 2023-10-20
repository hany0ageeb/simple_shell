#include "../shell.h"
#include "../str_list.h"
#include <stdio.h>
#include <stdlib.h>
static void test_get_paths(char **envp)
{
    size_t i = 0;

    char **paths = get_paths(envp);
    if (paths != NULL)
    {
        for (i = 0; paths[i] != NULL; ++i)
        {
            printf("%s\n", paths[i]);
        }
        free_str_list(&paths);
    }
}
static void test_find_full_path(char **envp)
{
    char **paths = get_paths(envp);
    char *full_path = find_full_path("ls", paths);
    printf("ls full path %s", full_path);
}
int main(__attribute__ ((unused)) int argc, __attribute__ ((unused)) char **argv, char **envp)
{
    printf("==============test shell.c============\n");
    test_get_paths(envp);
    printf("==========================================\n");
    test_find_full_path(envp);
    printf("============end test shell.c===========\n");
    exit(EXIT_SUCCESS);
}
