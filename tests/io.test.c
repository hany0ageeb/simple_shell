#include "../io.h"
#include "../string.h"
#include <stdio.h>
static void test_file_exists(const char *d, const char *f)
{
    bool_t ret;

    printf("==>file_exists function test\n");
    ret = file_exists(d, f);
    if (ret == TRUE)
    {
        printf("Ok!\n");
    }
    else{
        printf("Expected [TRUE] Got [FALSE]\n");
    }
    printf("==>end file_exists function test\n");
}
int main(int argc, char **argv)
{
    const char *d = copy_str(argv[1]);
    const char *f = copy_str(argv[2]);
    printf("============io.c tests=============\n");
    test_file_exists(d, f);
    if (d != NULL)
    free(d);
    if (f != NULL)
    free(f);
    printf("=========io.c tests end============\n");
    return (0);
}