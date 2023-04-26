#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define SHELL_NAME "hsh"

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;
	pid_t pid;
	int interactive = isatty(STDIN_FILENO);
	int status;

	while (1)
	{
		if (interactive && argc == 1)
			printf("($) ");
		characters_read = getline(&line, &bufsize, stdin);
		if (characters_read == -1)
			break;
		if (line[characters_read - 1] == '\n')
			line[characters_read - 1] = '\0';
		if (strcmp(line, "exit") == 0)
			break;
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			argv[0] = line;
			argv[1] = NULL;
			if (execve(argv[0], argv, NULL) == -1)
				fprintf(stderr, "./%s: 1: %s: not found\n", SHELL_NAME, argv[0]);
		}
		else
		{
			if (wait(&status) == -1)
			{
				perror("wait");
				exit(EXIT_FAILURE);
			}
		}
	}

	if (line)
		free(line);
	exit(EXIT_SUCCESS);
}
