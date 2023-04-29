#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/**
 * handle_cd - change directory
 * @dir: directory
 * @return void
 */
void handle_cd(char *dir)
	{
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

/**
 * handle_unsetenv- change directory
 * @name: directory
 * @return void
 */
void handle_unsetenv(char *name)
{
	if (unsetenv(name) == -1)
		perror("unsetenv");
}
