#include "../io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
	int fd = -1;
	char *lineptr = NULL;
	size_t n = 0;
	ssize_t n_read = -1;

	if (argc < 2)
	{
		printf("Error: Usage prog file_name\n");
		exit(EXIT_FAILURE);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	while ((n_read = _getline(&lineptr, &n, fd)) != -1)
	{
		_puts(lineptr);
	}
	close(fd);
	if (lineptr != NULL)
		free(lineptr);
	exit(EXIT_SUCCESS);
}

