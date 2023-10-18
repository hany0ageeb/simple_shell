#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *path = getenv("PATH");
	printf("%s\n", path);
	exit(EXIT_SUCCESS);
}

