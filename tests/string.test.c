#include "../string.h"
#include <stdio.h>
#include <string.h>
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
static void test_str_cmp()
{
    const char *str1 = "a", *str2 = "b";
    int ret;

    printf("==>str_cmp function tests\n");
    printf("==>use case: str_cmp(%s, %s)\n", str1, str2);
    ret = str_cmp(str1, str2);
    if (ret >= 0)
    {
        printf("Expected [-1] Got [%d]\n", ret);
    }
    else
    {
        printf("Ok!\n");
    }
    printf("==>use case: str_cmp(%s, %s)\n", str2, str1);
    ret = str_cmp(str2, str1);
    if (ret <= 0)
    {
        printf("Expected [1] GOT [%d]\n", ret);
    }
    else
    {
        printf("Ok!\n");
    }
    printf("==>use case: str_cmp(%s, %s)\n", str1, str1);
    ret = str_cmp(str1, str2);
    if (ret != 0)
    {
        printf("Expected [0] Got [%d]\n", ret);
    }
    else
    {
        printf("Ok!\n");
    }
    printf("==>str_cmp function tests end\n");
}
static void test_start_with()
{
    char *str = "HOME=/root";
    char *start_w = "HOME=";
    bool_t ret;

    printf("==>start_with function tests\n");
    printf("==>use case: start_with(%s, %s)\n", str, start_w);
    ret = start_with(str, start_w);
    if (ret == FALSE)
    {
        printf("Expected [TRUE] Got [FALSE]\n");
    }
    else
    {
        printf("Ok!\n");
    }
    printf("==>start_with function tests end\n");
}
static void test_count_str_list_len()
{
    char *str1 = "PLA:EPT";
    char c = ':';
    size_t count;
    printf("==>count_str_list_len function tests\n");
    printf("==>use case: count_str_list_len(%s, %c)\n", str1, c);
    count = count_str_list_len(str1, c);
    if (count == 2)
    {
        printf("Ok!\n");
    }
    else
    {
        printf("Expected [2] Got [%lu]", count);
    }
    printf("==>count_str_list_len function tests end\n");
}
static void test_split_str()
{
    char *str1 = "PLA:TTA";
    char c = ':';
    char **parts = NULL;
    int i = 0;

    printf("==>split_str function tests\n");
    printf("==>use case: split_str(%s, %c)\n", str1, c);
    parts = split_str(str1, c);
    if (parts != NULL)
    {
        printf("Expected [PLA, TTA] Got [");
        while (parts[i] != NULL)
        {
            printf("%s, ", parts[i]);
	    i++;
        }
        printf("]\n");
	i = 0;
	while (parts[i] != NULL)
	{
		free(parts[i]);
		parts[i] = NULL;
		i++;
	}
	free(parts);
	parts = NULL;
    }
    printf("==>split_str function tests end\n");
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
static void test_str_equals()
{
    char *str1 = "Hello";
    char *str2 = "Hello";
    int ret;

    printf("==>str_equals function test\n");
    printf("==>use case: str_equals(%s, %s)", str1, str2);
    ret = strcmp(str1, str2);
    if (ret == 0 && str_equals(str1, str2) == TRUE)
    {
        printf("Ok!\n");
    }
    else
    {
        printf("Got [False] Expected [True]\n");
    }
    printf("==>str_equals function test end\n");
}
static void test_index_of()
{
    char *str = "Hello World!";
    size_t len;
    int index;

    printf("==>index_of function test\n");
    len = strlen(str);
    printf("use case: index_of(%s, 0, %lu, W)\n", str, len - 1);
    index = index_of(str, 0, len - 1, 'W');
    if (index == 6)
    {
        printf("Ok!\n");
    }
    else
    {
        printf("Expected [6] Got [%d]", index);
    }
    printf("==>index_of function test end\n");
}
int main()
{
    printf("=======string.c tests====\n");
    test_str_len();
    printf("===============================\n");
    test_copy_str();
    printf("================================\n");
    test_concat_str();
    printf("=================================\n");
    test_sub_str();
    printf("==================================\n");
    test_str_cmp();
    printf("==================================\n");
    test_start_with();
    printf("===================================\n");
    test_count_str_list_len();
    printf("===================================\n");
    test_split_str();
    printf("=================================\n");
    test_contains_char();
    printf("==================================\n");
    test_str_equals();
    printf("===========string.c tests end=====\n");
    exit(EXIT_SUCCESS);
}
