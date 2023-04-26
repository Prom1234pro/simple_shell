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
    char *args[10];
    int i;
	char *token;

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
            if (argc > 1)
            {
                int exit_status = atoi(argv[1]);
                exit(exit_status);
            }
            else
            {
                exit(EXIT_SUCCESS);
            }
        }
        token = line;
        i = 0;
        while (*token != '\0')
        {
            while (*token == ' ')
                token++;
            if (*token == '\0')
                break;
            args[i++] = token;
            while (*token != ' ' && *token != '\0')
                token++;
            if (*token == '\0')
                break;
            *token++ = '\0';
        }
        args[i] = NULL;
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
                fprintf(stderr, "./%s: 1: %s: not found\n", SHELL_NAME, args[0]);
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
