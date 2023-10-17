#include "../string.h"
#include <stdio.h>
#include <stdlib.h>
static void test_str_len()
{
    char *empt_str = "", *null_str = NULL, *normal_str = "test";
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
    char *empt_str = "", *null_str = NULL, *normal_str = "test";
    char *tmp;

    printf("==>copy_str function tests\n");
    printf("===>use case: Empty string\n");
    tmp = copy_str(empt_str);
    printf("original len: %lu\tcopy len: %lu\n", str_len(empt_str), str_len(tmp));
    printf("original string: %s\tcop string: %s\n", empt_str, tmp);
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
    printf("================================================\n");
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
    printf("Expected [NULL]\tGot[%s]\n", tmp);
    if (tmp != NULL)
    {
	    free(tmp);
	    tmp = NULL;
    }
    printf("===>use case: li = 0, hi = 1, string = %s\n", str);
    tmp = sub_str(str, 0, 1);
    printf("Expected [%s] Got [%s]\n", "te", tmp);
    if (tmp != NULL)
    {
	    free(tmp);
	    tmp = NULL;
    }
    printf("==>sub_str function tests end\n");
    printf("=======================================\n");
}
static void test_concat_str()
{
	char *empty_str = "", *null_str = NULL, *normal_str1 = "hello ", *normal_str2 = "world";
	char *str;

	printf("==>concat_str function tests\n");
	printf("===>should return null if str1 is null and str2 is null\n");
	str = concat_str(null_str, null_str);
	if (str == NULL)
	{
		printf("Expected [NULL] GOT [NULL]\n");
	}
	else
	{
		printf("Failed: Expected [NULL] Got [%s]\n", str);
		free(str);
		str = NULL;
	}
	printf("===>should return empty string if both str1 and str2 are empty\n");
	str = concat_str(empty_str, empty_str);
	printf("Expected [%s] Got[%s]\n", "", str);
	if (str != NULL)
	{
		free(str);
		str = NULL;
	}
	printf("===>str1=%s, str2=%s should return hello world\n", normal_str1, normal_str2);
	str = concat_str(normal_str1, normal_str2);
	printf("Expected [%s] Got [%s]\n", "hello world", str);
	if (str != NULL)
	{
		free(str);
		str = NULL;
	}
	printf("==>concat_str tests end\n");
}
static void test_contains_char()
{
	const char *str = "PWD";
	const char *str1 = "./path/to/file";
	char c = '/';

	printf("==>contains_str function tests\n");
	printf("==>use case: str = %s , c = %c\n", str, c);
	if (contains_char(str, c))
	{
		printf("contains_char returned True expected false\n");
	}
	else
	{
		printf("contains_char returned false expected false\n");
	}
	printf("==>use case: str = %s , c = %c\n", str1, c);
	if (contains_char(str1, c))
	{
		printf("Got [TRUE] Expected [TRUE]\n");
	}
	else
	{
		printf("Got [FALSE] Expected [TRUE]\n");
	}
	printf("==>contains_char function tests end\n");
}
int main()
{
    printf("=======string.c tests====\n");
    test_str_len();
    test_copy_str();
    test_concat_str();
    test_sub_str();
    printf("=================================\n");
    test_contains_char();
    printf("===========string.c tests end=====\n");
    exit(EXIT_SUCCESS);
}
