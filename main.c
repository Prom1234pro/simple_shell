#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

#define BUFFER_SIZE 1024
#define SHELL_NAME "hsh"

/**
 * main - main entry to code
 * @argc: the name of the environment variable to unset
 * @argv: the name of the environment variable to unset
 * @return 0 on success, -1 on failure
 */
int main(int argc, char *argv[])
{
    
    int interactive = isatty(STDIN_FILENO);
    int status = 0;
	char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters_read;

    if (argc == 2)
    {
		int stat = execute_commands_from_file(argv[1], &status);
		return (stat);
    }

    while (1)
    {
        if (interactive)
            printf("($) ");
        characters_read = getline(&line, &bufsize, stdin);
        if (characters_read == -1)
            break;
        if (line[characters_read - 1] == '\n')
            line[characters_read - 1] = '\0';
        if (strcmp(line, "exit") == 0)
        {
            exit(status);
        }
        else if (strncmp(line, "exit ", 5) == 0)
        {
            char *code_str = line + 5;
            status = atoi(code_str);
            exit(status);
        }
        else if (strncmp(line, "cd ", 3) == 0)
        {
            char *dir = line + 3;
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

    if (line)
        free(line);
    exit(status);
}
