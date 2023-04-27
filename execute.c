#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * execute_commands_from_file- change directory
 * @filename: directory
 * @status: directory
 * @return an integer
 */
int execute_commands_from_file(const char *filename, int *status)
{
	FILE *fp = fopen(filename, "r");
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t characters_read;
	char *dir;

		if (fp == NULL)
		{
			perror("Error opening file");
			return (EXIT_FAILURE);
		}
		while ((characters_read = getline(&line, &bufsize, fp)) != -1)
		{
			if (line[characters_read - 1] == '\n')
				line[characters_read - 1] = '\0';
			if (strncmp(line, "cd ", 3) == 0)
			{
				dir = line + 3;
				handle_cd(dir);
			}
			else if (strncmp(line, "setenv ", 7) == 0)
			{
				char *name = line + 7;
				char *value = strchr(name, ' ');

				if (value == NULL)
				{
					fprintf(stderr, "./%s: setenv: missing value\n", SHELL_NAME);
					continue;
				}
				*value++ = '\0';
				if (setenv(name, value, 1) == -1)
					perror("setenv");
			}
			else if (strncmp(line, "unsetenv ", 9) == 0)
			{
				char *name = line + 9;

				handle_unsetenv(name);
			}
			else
				execute(line);
		}
		fclose(fp);
		if (line)
			free(line);
		return (*status);
}
