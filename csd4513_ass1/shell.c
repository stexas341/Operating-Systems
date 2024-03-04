//Stylianos Sfakianakis csd4513

#include "shell.h"

void show_prompt() {
    char *cwd = getcwd(NULL, 0);
    char *user = getlogin();
    printf("csd4513-hy345sh@%s:%s~$ ", user, cwd);
}

void change_dir(char *path) {
    if (chdir(path) == -1) {
        perror("Path not found");
    }
}

void basicexec(char **args) {
    pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("Exec error");
        exit(1);
    } else if (pid < 0) {
        perror("Fork error");
    } else {
        wait(NULL);
    }
}

char **split_args(char *input, char **args) {
    char *tok;
    int i = 0;
    args = (char **)malloc(1000 * sizeof(char *));
    tok = strtok(input, " ");

    while (tok != NULL) {
        args[i] = tok;
        i++;
        tok = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

char *read_input() {
    char *cmds = (char *)malloc(1000 * sizeof(char));

    while (1) {
        show_prompt();
        int i = 0;
        int c;
        while ((c = getchar()) != '\n') {
            cmds[i] = c;
            i++;
        }
        cmds[i] = '\0';
        if (strcmp(cmds, "exit") == 0 || strcmp(cmds, "quit") == 0) {
            break;
        } else {
            process_input(cmds);
        }
    }
    free(cmds);
    return NULL;
}

void process_input(char *input) {
    char *cmds[100];
    int cmd_count = 0;

    char *tok = strtok(input, ";");
    while (tok != NULL) {
        cmds[cmd_count++] = tok;
        tok = strtok(NULL, ";");
    }
    int in = 0;

    for (int i = 0; i < cmd_count; i++) {
        if (strstr(cmds[i], "|") != NULL) {
            char *pipe_cmds[100];
            int pipe_cmd_count = 0;
            char *pipe_tok = strtok(cmds[i], "|");
            while (pipe_tok != NULL) {
                pipe_cmds[pipe_cmd_count++] = pipe_tok;
                pipe_tok = strtok(NULL, "|");
            }

            int pipes[2];
            int prev_input_fd = in;

            for (int j = 0; j < pipe_cmd_count; j++) {
                char **arg = split_args(pipe_cmds[j], NULL);
                if (j < pipe_cmd_count - 1) {
                    pipe(pipes);
                    execpipes(arg, prev_input_fd, pipes[1]);
                    close(pipes[1]);
                    prev_input_fd = pipes[0];
                } else {
                    execpipes(arg, prev_input_fd, 1);
                }
            }
        } else {
            char **arg = split_args(cmds[i], NULL);
            if (strcmp("cd", arg[0]) == 0 || strcmp("chdir", arg[0]) == 0) { // + cd
                change_dir(arg[1]);
            } else {
                int redir_in = 0;
                int redir_out = 0;
                int append_out = 0;
                char *ifile = NULL;
                char *ofile = NULL;

                for (int j = 0; arg[j] != NULL; j++) {
                    if (strcmp(arg[j], "<") == 0) {
                        redir_in = 1;
                        ifile = arg[j + 1];
                        arg[j] = NULL;
                        j++;
                    } else if (strcmp(arg[j], ">") == 0) {
                        redir_out = 1;
                        ofile = arg[j + 1];
                        arg[j] = NULL;
                        j++;
                    } else if (strcmp(arg[j], ">>") == 0) {
                        redir_out = 1;
                        append_out = 1;
                        ofile = arg[j + 1];
                        arg[j] = NULL;
                    }
                }

                if (redir_in) {
                    int in = open(ifile, O_RDONLY);
                    dup2(in, STDIN_FILENO);
                    close(in);
                }

                if (redir_out) {
                    pid_t child = fork();

                    if (child == 0) {
                        int out;
                        if (append_out) {
                            out = open(ofile, O_WRONLY | O_CREAT | O_APPEND, 0666);
                        } else {
                            out = open(ofile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                        }
                        dup2(out, STDOUT_FILENO);
                        close(out);

                        if (execvp(arg[0], arg) < 0) {
                            perror("Exec error");
                            exit(1);
                        }
                    } else if (child < 0) {
                        perror("Fork error");
                    }
                } else {
                    basicexec(arg);
                }
            }
        }
    }
}

void execpipes(char **arg, int in, int out) {
    pid = fork();
    if (pid == 0) {
        if (in != 0) {
            dup2(in, 0);
            close(in);
        }
        dup2(out, 1);
        if (execvp(arg[0], arg) < 0) {
            perror("Exec error");
            exit(1);
        }
    } else if (pid < 0) {
        printf("Fork error\n");
    } else {
        wait(NULL);
    }
}

int main() {
    char *input = read_input();
    return 0;
}
