#include "shell.h"

#include "ANSI-color-codes.h"
#include "server.h"

int command_EXIT(char **args);
int command_ECHO(char **args);
int command_TCP_PORT(char **args);
int command_LOCAL(char **args);
int command_DIR(char **args);
int command_CD(char **args);
int command_COPY(char **args);
int command_DELETE(char **args);
int command_HELP(char **args);

shell_command shell_commands[] = {
    {"EXIT", "Exit the shell", command_EXIT},
    {"ECHO", "Echo a string", command_ECHO},
    {"TCP PORT", "Pipes stdout to a TCP socket", command_TCP_PORT},
    {"LOCAL", "Cancels the effect of the TCP PORT Command", command_LOCAL},
    {"DIR", "Lists all files in current working directory", command_DIR},
    {"CD", "Change directory", command_CD},
    {"COPY", "Copies a file from SRC to DST", command_COPY},
    {"DELETE", "Deletes a file", command_DELETE},
    {"HELP", "Displays this help", command_HELP}
};

int number_of_commands() {
    return sizeof(shell_commands) / sizeof(shell_command);
}

// ==========================COMMANDS==========================

// Section a - EXIT - Exit the shell
int command_EXIT(char **args) {
    return 0;
}

// Section c - ECHO - print to stdout
int command_ECHO(char **args) {
    int i;
    for (i = 0; args[i] != NULL; ++i) {
        fprintf(stdout, "%s", args[i]);
        if (args[i + 1] != NULL) {
            fprintf(stdout, " ");
        }
    }
    fprintf(stdout, "\n");
    return 1;
}

// Section d - TCP PORT - Pipes stdout to a TCP socket
int command_TCP_PORT(char **args) {
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCKET_PORT);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    dup2(sockfd, 1);
    dup2(sockfd, 2);

    close(sockfd);

    return 1;
}

// Section e - LOCAL - Cancels the effect of the TCP PORT Command
int command_LOCAL(char **args) {
    dup2(0, 1);
    dup2(0, 2);
    return 1;
}

// Section f - DIR - List all files in current working directory
int command_DIR(char **args) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *colour = WHT;
            if (dir->d_type == DT_BLK) {
                colour = BLKB BYEL;
            } else if (dir->d_type == DT_CHR) {
                colour = BLKB BYEL;
            } else if (dir->d_type == DT_DIR) {
                colour = BBLU;
            } else if (dir->d_type == DT_FIFO) {
                colour = BLKB YEL;
            } else if (dir->d_type == DT_LNK) {
                colour = BCYN;
            } else if (dir->d_type == DT_REG) {
                colour = BGRN;
            } else if (dir->d_type == DT_SOCK) {
                colour = BMAG;
            } else if (dir->d_type == DT_UNKNOWN) {
                colour = WHT;
            }
            printf("%s%s" COLOR_RESET "\n", colour, dir->d_name);
        }
        closedir(d);
    }
    return 1;
}

// Section g - CD - Change current working directory using chdir which is a system call.
int command_CD(char **args) {
    char *path = ".";
    if (args[1] != NULL) {
        path = args[1];
    }
    if (chdir(path) != 0) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
    }
    return 1;
}

// Section j - COPY - Copies a file from SRC to DST - fopen, fread, fwrite, fclose are not system calls.
int command_COPY(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Error: Usage: COPY SRC DST\n");
        return 1;
    }
    FILE *src = fopen(args[1], "r");
    if (src == NULL) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 1;
    }
    FILE *dst = fopen(args[2], "w");
    if (dst == NULL) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 1;
    }
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int read = 0;
    while ((read = fread(buffer, sizeof(char), BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, sizeof(char), read, dst);
    }
    free(buffer);
    fclose(src);
    fclose(dst);
    return 1;
}

// Section k - DELETE - Deletes a file using unlink which is a system call.
int command_DELETE(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Error: Usage: DELETE FILE\n");
        return 1;
    }
    if (unlink(args[1]) != 0) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return 1;
    }
    return 1;
}

// HELP
int command_HELP(char **args) {
    int i;
    if (args[1] == NULL) {
        for (i = 0; i < number_of_commands(); ++i) {
            fprintf(stdout, "%*s %*s\n", -15, shell_commands[i].command, -15, shell_commands[i].description);
        }
    } else {
        for (i = 0; i < number_of_commands(); ++i) {
            if (multi_word_command(shell_commands[i].command, args)) {
                fprintf(stdout, "%*s %*s\n", -15, shell_commands[i].command, -15, shell_commands[i].description);
            }
        }
        fprintf(stdout, "%s: command not found\n", args[1]);
    }
    return 1;
}

// ============================================================

// cumulative concatenate all args into one string
int multi_word_command(char *shell_cmd, char **args) {
    int i;
    char *command;
    int word_count = 0;
    int is_eq = 0;
    for (i = 0; args[i] != NULL; ++i) {
        if (!is_eq) {
            if (i == 0) {
                command = (char *)malloc(sizeof(char) * (strlen(args[0]) + 1));
                strcpy(command, args[0]);
            } else {
                command = realloc(command, sizeof(char) * (strlen(command) + strlen(args[i]) + 2));
                strcat(command, " ");
                strcat(command, args[i]);
            }
            word_count++;
            is_eq = strcmp(command, shell_cmd) == 0;
        }
    }
    // if (is_eq) {
    //     memmove(args, args + word_count, sizeof(char *) * (i - word_count + 1));
    // }

    free(command);
    return is_eq;
}

char *shell_readline(void) {
    int bufsize = BUFFER_SIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int current_char;

    if (!buffer) {
        fprintf(stderr, "SHELL ALLOCATION ERROR (char *shell_readline)\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // read a char from stdin
        current_char = getc(stdin);

        // if the char is EOF or newline replace it with null char and return the buffer
        if (current_char == EOF || current_char == '\n') {
            buffer[position] = '\0';
            return buffer;
        }
        buffer[position] = current_char;

        position++;

        // if the buffer is full reallocate it
        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "SHELL ALLOCATION ERROR (char *shell_readline)\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **shell_tokeniser(char *line) {
    int bufsize = BUFFER_SIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "SHELL ALLOCATION ERROR (char **shell_tokeniser)\n");
        exit(EXIT_FAILURE);
    }

    // tokenize the line
    token = strtok(line, DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        // if the buffer is full reallocate it
        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "SHELL ALLOCATION ERROR (char **shell_split_line)\n");
                exit(EXIT_FAILURE);
            }
        }

        // get the next token
        token = strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

// section h - If all else fails run the command using system() which is a system call
// section i - If all else fails run the command using fork, exec, wait
int shell_launch(char **args) {
    if (args[0] == NULL) {
        return 1;
    }
    // use system() to launch the command - section h - which is a system call
    // // join all args into one string
    // int i;
    // char *command = (char *)malloc(sizeof(char) * (strlen(args[0]) + 1));
    // strcpy(command, args[0]);
    // for (i = 1; args[i] != NULL; ++i) {
    //     command = realloc(command, sizeof(char) * (strlen(command) + strlen(args[i]) + 2));
    //     strcat(command, " ");
    //     strcat(command, args[i]);
    // }
    // int status = system(command);
    // if (status == -1) {
    //     fprintf(stderr, "Error: %s\n", strerror(errno));
    // }
    // free(command);

    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "SHELL ERROR: %s\n", strerror(errno));
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        fprintf(stderr, "SHELL ERROR: %s\n", strerror(errno));
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int shell_executor(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    int i;
    for (i = 0; i < number_of_commands(); ++i) {
        if (multi_word_command(shell_commands[i].command, args)) {
            return shell_commands[i].function(args);
        }
        // if (strcmp(args[0], shell_commands[i].command) == 0) {
        //     return shell_commands[i].function(args);
        // }
    }

    return shell_launch(args);
}

void shell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        // Section a - print prompt
        // printf("yes master? ");
        char prompt[BUFFER_SIZE * 4];
        // Section b - print current working directory
        getcwd(prompt, sizeof(prompt));
        // printf(RED "%s" COLOR_RESET "$ ", prompt);
        fprintf(stdout, RED "%s" COLOR_RESET "$ ", prompt);
        fflush(stdout);

        line = shell_readline();
        args = shell_tokeniser(line);
        status = shell_executor(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv) {
    shell_loop();

    // char *line;
    // char **args;

    // line = shell_readline();
    // args = shell_tokeniser(line);

    // for (int i = 0; args[i] != NULL; i++) {
    //     printf("1) %s of length: %ld\n", args[i], strlen(args[i]));
    // }

    // // for (int i = 0; i < number_of_commands(); ++i) {
    // //     if (multi_word_command(shell_commands[i].command, args) == 1) {
    // //         printf("%s\n", shell_commands[i].command);
    // //     }
    // // }
    // printf("%d\n", multi_word_command(shell_commands[1].command, args));

    // for (int i = 0; args[i] != NULL; i++) {
    //     printf("2) %s of length: %ld\n", args[i], strlen(args[i]));
    // }

    // free(line);
    // free(args);

    return EXIT_SUCCESS;
}