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
		free_token_list(&tok_lst);
		return (session->status);
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
	char *line = NULL;
	size_t n = 0;
	ssize_t n_read = 0, run_result = 0;

	if (isatty(STDIN_FILENO))
	{
		_puts(session->prompt);
		n_read = getline(&line, &n, stdin);
		while (n_read != -1)
		{
			run_result = run(line, session);
			if (session->exit_request == TRUE)
			{
				if (line != NULL)
					free(line);
				return (session->status);
			}
			_puts(session->prompt);
			n_read = getline(&line, &n, stdin);
		}
		if (line != NULL)
		{
			free(line);
			line = NULL;
		}
	}
	else
	{
		n_read = getline(&line, &n, stdin);
		while (n_read > 0)
		{
			run_result = run(line, session);
			if (session->exit_request == TRUE)
			{
				free(line);
				return (session->status);
			}
			n_read = getline(&line, &n, stdin);
		}
		if (line != NULL)
		{
			free(line);
			line = NULL;
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
	ssize_t n_read = 0;

	if (fd == -1)
	{
		/**print error message*/
		_fputs(session->sh_name, STDERR_FILENO);
		_fputs(": 0: Can't open ", STDERR_FILENO);
		_fputs(file, STDERR_FILENO);
		_fputs("\n", STDERR_FILENO);
		return(127);
	}
	while ((n_read = _getline(&line, &n, fd)) != -1)
	{
		run_result = run(line, session);
		if (session->exit_request == TRUE)
		{
			close(fd);
			if (line != NULL)
				free(line);
			return (session->status);
		}
	}
	close(fd);
	if (line != NULL)
		free(line);
	return (run_result);
}
