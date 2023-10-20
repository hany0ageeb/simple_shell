#include "../str_list.h"
#include <stdio.h>
#include <stdlib.h>
static void test_create_str_list()
{
	size_t i;
	char **lst = create_str_list(2, "/root", "/root/simple_shell");
	if (lst != NULL)
	{
		for (i = 0; lst[i] != NULL; ++i)
		{
			printf("%s\n", lst[i]);
		}
		for (i = 0; lst[i] != NULL; ++i)
		{
			free(lst[i]);
			lst[i] = NULL;
		}
		free(lst);
		lst = NULL;
	}
}
static void test_free_str_list()
{
	char **lst = create_str_list(2, "/root", "/root/simple_shell");
	free_str_list(&lst);
}
int main()
{
	printf("=================str_list.c tests\n");
	test_create_str_list();
	test_free_str_list();
	printf("=================str_list.c tests end\n");
	return (0);
}

