#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * main - main function
 * Return:int
 */

int main(void)
{
	char cmd[100];
	char *args[2];
	pid_t pid;

	while (1)
	{
		printf("$ ");
		if (fgets(cmd, 100, stdin) == NULL)
		{
			printf("\n");
			exit(0);
		}
		if (cmd[strlen(cmd) - 1] == '\n')
			cmd[strlen(cmd) - 1] = '\0';
		if (strlen(cmd) == 0)
			continue;
		if (strchr(cmd, ' ') != NULL)
		{
			printf("./shell: No such file or directory.\n");
			continue;
		}
		if (access(cmd, X_OK) == -1)
		{
			printf("./shell: No such file or directory.\n");
			continue;
		}
		pid = fork();
		if (pid == 0)
		{
			args[0] = cmd;
			args[1] = NULL;
			execve(cmd, args, NULL);
			printf("./shell: No such file or directory.\n");
			exit(1);
		} else
		{
			waitpid(pid, NULL, 0);
		}
	}
	return (0);
}
