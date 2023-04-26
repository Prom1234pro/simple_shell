#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define SHELL_NAME "hsh"

/**
 * main - main function
 * @argc: argument count
 * @argv: argument vector
 * Return: int
 */

int main(int argc, char **argv, char **envp)
{
	char *line = NULL, *token, *path, *cmd;
	size_t bufsize = 0;
	ssize_t characters_read;
	pid_t pid;
	int interactive = isatty(STDIN_FILENO);
	int status;
	int i;

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
		cmd = strtok(line, " ");
		if (!cmd)
			continue;
		path = strtok(getenv("PATH"), ":");
		while (path)
		{
			char *command_path = malloc(strlen(path) + strlen(cmd) + 2);
			sprintf(command_path, "%s/%s", path, cmd);
			if (access(command_path, F_OK) != -1)
			{
				pid = fork();
				if (pid == -1)
				{
					perror("fork");
					exit(EXIT_FAILURE);
				}
				else if (pid == 0)
				{
					token = strtok(NULL, " ");
					i = 1;
					while (token)
					{
						argv[i] = token;
						token = strtok(NULL, " ");
						i++;
					}
					argv[i] = NULL;
					if (execve(command_path, argv, envp) == -1)
						fprintf(stderr, "./%s: 1: %s: not found\n", SHELL_NAME, cmd);
				}
				else
				{
					if (wait(&status) == -1)
					{
						perror("wait");
						exit(EXIT_FAILURE);
					}
					break;
				}
			}
			free(command_path);
			path = strtok(NULL, ":");
		}
		if (!path)
			fprintf(stderr, "./%s: 1: %s: not found\n", SHELL_NAME, cmd);
	}

	if (line)
		free(line);
	exit(EXIT_SUCCESS);
}
