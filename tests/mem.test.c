#include "../mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static void test_copy_mem()
{
	const char *str = "str";
	char *cpy = malloc(sizeof(char) * 4);

	printf("==>test copy_mem function\n");
	copy_mem(cpy, str, 4);
	printf("Orignal Mem [%s] Copied Memory [%s]\n", str, cpy);
	free(cpy);
	printf("==>end test copy_mem function\n");
}
static void test_realloc()
{
	char *str = malloc(sizeof(char) * 4);

	strcpy(str, "str");
	printf("==>test _realloc function\n");
	printf("Before realloc: str = %s, len = %lu\n", str, strlen(str));
	str = _realloc(str, 4, 5);
	printf("calling _realloc old size = 4, new size = 5\n");
	str[3] = '1';
	str[4] = '\0';
	printf("After realloc: str = %s, len = %lu\n", str, strlen(str));
	printf("Before realloc str = %s, len = %lu\n", str, strlen(str));
	printf("calling realloc old_size = 5, new _size = 3\n");
	str = _realloc(str, 5, 3);
	str[2] = '\0';
	printf("After realloc str = %s, len = %lu\n", str, strlen(str));
	if (str != NULL)
	{
		free(str);
		str = NULL;
	}
	printf("==>end tests _realloc function\n");
}
int main(void)
{
	printf("=========================================\n");
	printf("==>mem.c functions tests\n");
	test_copy_mem();
	printf("==============================================\n");
	test_realloc();
	printf("==============================================\n");
	printf("==>end of mem.c function tests\n");
	exit(EXIT_SUCCESS);
}

