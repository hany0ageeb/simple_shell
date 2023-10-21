#include "../io.h"
#include "../string.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static void test_getline()
{
	char *lineptr = NULL;
	size_t n = 0;
	ssize_t n_read;
	printf("====>test _getline function\n");
	printf("Enter: ");
	while ((n_read = _getline(&lineptr, &n, STDIN_FILENO))!=-1)
	{
		printf("\nYOU ENTERED: %s LENGTH = %lu\n", lineptr, n);
		printf("Enter: ");
	}
	if (lineptr != NULL)
		free(lineptr);
}
int main()
{
	printf("=====================io.c tests======================\n");
	printf("====>test _getline function\n");
	test_getline();
	printf("=====================================================\n");
	return (0);
}
