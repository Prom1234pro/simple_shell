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
    int status = 0;
    char *token;
    char *args[10];
    int i;

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
        else if (strncmp(line, "cd ", 3) == 0)
        {
            char *dir = line + 3;
            if (*dir == '\0')
                dir = getenv("HOME");
            else if (strcmp(dir, "-") == 0)
            {
                dir = getenv("OLDPWD");
                printf("%s\n", dir);
            }
            if (dir && chdir(dir) == -1)
                perror(dir);
            else
            {
                setenv("OLDPWD", getenv("PWD"), 1);
                setenv("PWD", getcwd(NULL, 0), 1);
            }
        }
        else
        {
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
    }

    if (line)
        free(line);
    exit(status);
}
