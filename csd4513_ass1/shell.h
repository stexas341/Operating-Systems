//Stylianos Sfakianakis csd4513

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

pid_t pid;

void show_prompt();
void basicexec(char **args);
char **split_args(char *input, char **args);
void change_dir(char *path);
char *read_input();
void process_input(char *input);
void execpipes(char **arg, int in, int out);