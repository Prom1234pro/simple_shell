#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define SHELL_NAME "hsh"

int main()
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters_read;
    pid_t pid;
    int interactive = isatty(STDIN_FILENO);
    int status;
    char *token;
    char *args[10];
    int i;
    char *path;
    char *pathenv = getenv("PATH");
    char *pathdir;

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
            break;
        i = 0;
        token = strtok(line, " ");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
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
            pathdir = strtok(pathenv, ":");
            while (pathdir != NULL)
            {
                path = malloc(strlen(pathdir) + strlen(args[0]) + 2);
                sprintf(path, "%s/%s", pathdir, args[0]);
                if (access(path, X_OK) == 0)
                {
                    if (execve(path, args, __environ) == -1)
                        fprintf(stderr, "./%s: 1: %s: not found\n", SHELL_NAME, args[0]);
                }
                free(path);
                pathdir = strtok(NULL, ":");
            }
            fprintf(stderr, "./%s: 1: %s: not found\n", SHELL_NAME, args[0]);
            exit(EXIT_FAILURE);
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
