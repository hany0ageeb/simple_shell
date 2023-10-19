#include "bool.h"
#include "shell.h"
#include "token.h"
#include "session.h"
#include <stdio.h>
#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
static int run(const char *src, sh_session_t *session);
static int run_prompt(sh_session_t *session);
static int run_file(const char *file, sh_session_t *session);
/**
 * main - Entry point
 * @argc: argc
 * @argv: argv
 * @envp: envp
 * Return: 0 on sucess
 */
int main(int argc, char **argv, char **envp)
{
	sh_session_t *session = create_session(argv[0], envp);
	int ret;

	if (session == NULL)
	{
		perror("Create session");
		return (-1);
	}
	if (argc == 1)
	{
		ret = run_prompt(session);
	}
	else if (argc == 2)
	{
		ret = run_file(argv[1], session);
	}
	else
	{
		/*TODO: */
		ret = EXIT_FAILURE;
	}
	if (session != NULL)
	{
		free_session(&session);
	}
	return (ret);
}
/**
 * run - run
 * @src: source code
 * @session: shell session
 * Return: 0 on success
 */
static int run(const char *src, sh_session_t *session)
{
	token_list_t *tok_lst = NULL;

	if (scan_tokens(src, &tok_lst, session->sh_name))
	{
		parse_tokens(tok_lst, session);
	}
	else
	{
		return (-1);
	}
	return (0);
}
/**
 * run_prompt - run prompt
 * @session: shell session
 * Return: 0 on success
 */
static int run_prompt(sh_session_t *session)
{
	char *line;
	size_t n = 0;
	ssize_t n_read = 0, run_result = 0;

	if (isatty(STDIN_FILENO))
	{
		while (TRUE)
		{
			_puts(session->prompt);
			n_read = getline(&line, &n, stdin);
			if (n_read == -1 && errno != 0)
			{
				return (-1);
			}
			run_result = run(line, session);
		}
	}
	else
	{
		n_read = getline(&line, &n, stdin);
		while (n_read > 0)
		{
			run_result = run(line, session);
			n_read = getline(&line, &n, stdin);
		}
		return (run_result);
	}
	return (run_result);
}
/**
 * run_file - run file
 * @file: file to run
 * @session: session
 * Return: 0 on success
 */
static int run_file(const char *file, sh_session_t *session)
{
	int fd = open(file, O_RDONLY);
	char *line = NULL;
	int run_result = 0;
	size_t n = 0;
	ssize_t n_read;

	if (fd == -1)
	{
		perror("Open File");
		return (-1);
	}
	while ((n_read = _getline(&line, &n, fd)) != -1)
	{
		run_result = run(line, session);
	}
	if (line != NULL && errno == 0)
	{
		run_result = run(line, session);
		free(line);
	}
	return (run_result);
}

