#include "../string.h"
#include <stdio.h>
#include <stdlib.h>
static void test_str_len()
{
    char *empt_str = "", *null_str = NULL, normal_str = "test";
    char *tmp;
    size_t len;

    printf("==>str_len function tests\n");
    printf("===>use case: Empty string\n");
    len = str_len(empt_str);
    printf("GOT[%lu]\tExpected[0]\n", len);
    printf("===>use case: NULL string\n");
    len = str_len(null_str);
    printf("GOT[%lu]\tExpected[0]\n", len);
    printf("===>use case: normal string\n");
    len = str_len(normal_str);
    printf("string: %s\tExpected[4] GOT [%lu]\n", normal_str, len);
    printf("==>str_len function tests end\n");
}
static void test_copy_str()
{
    char *empt_str = "", *null_str = NULL, normal_str = "test";
    char *tmp;
    size_t len;

    printf("==>copy_str function tests\n");
    printf("=======string.c tests ends====\n");
    printf("===>use case: Empty string\n");
    tmp = copy_str(empt_str);
    printf("original len: %lu\tcopy len: %lu\n", str_len(empt_str), str_len(tmp));
    printf("original string: %s\tcop string: %s\n");
    if (tmp != NULL)
    free(tmp);
    tmp = NULL;
    printf("===>use case: NULL string\n");
    tmp = copy_str(null_str);
    if (tmp == null_str)
    {
        printf("EXPECTED [NULL]\tGOT [NULL]\n");
    }
    else
    {
        printf("EXPECTED [NULL]\tGOT [%s]\n", tmp);
        free(tmp);
        tmp = NULL;
    }
    tmp = copy_str(normal_str);
    printf("Expected [%s]\tGot[%s]\n", normal_str, tmp);
    printf("Original str len [%lu] Copy str len [%lu]\n", str_len(normal_str), str_len(tmp));
    if (tmp != NULL)
    {
        free(tmp);
        tmp = NULL;
    }
    printf("==>copy_str function tests end.\n");
}
static void test_sub_str()
{
    char *tmp;
    const char *str = "test";

    printf("==>sub_str function tests\n");
    printf("===>use case: li = 0, hi = 0, string = %s\n", str);
    tmp = sub_str(str, 0, 0);
    printf("Expected [t] Got[%s]\n", tmp);
    if (tmp != NULL)
    {
        free(tmp);
        tmp = NULL;
    }
    printf("===>use case: li = 1, hi = 0, string = %s\n", str);
    tmp = sub_str("test", 1, 0);
    printf("Expected [%s]\tGot[%s]\n", NULL, tmp);
    printf("===>use case: li = 0, hi = 1, string = %s\n", str);
    printf("==>sub_str function tests end\n");
}
static void test_concat_str()
{

}
int main()
{
    printf("=======string.c tests====\n");
    test_str_len();
    test_copy_str();
    test_concat_str();
    test_sub_str();
    printf("===========string.c tests end=====\n");
    exit(EXIT_SUCCESS);
}