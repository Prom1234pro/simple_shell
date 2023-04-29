#ifndef SHELL_H
#define SHELL_H

#define SHELL_NAME "hsh"

void handle_cd(char *dir);
void handle_unsetenv(char *name);
void execute(char *line);
int execute_commands_from_file(const char *filename, int *status);

#endif
