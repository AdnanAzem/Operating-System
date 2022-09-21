#ifndef SHELL_HEADER
#define SHELL_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024      // Size of the buffer
#define DELIMITERS "\n\r\t "  // delimiters for tokenizing newline, carriage return, tab, space respectively

int multi_word_command(char* shell_cmd, char **args);
void shell_loop(void);
int shell_executor(char **args);
char **shell_tokeniser(char *line);
char *shell_readline(void);
int shell_launch(char **args);

typedef struct shell_command {
    char *command;
    char *description;
    int (*function)(char **args);
} shell_command;

extern shell_command shell_commands[];

int number_of_commands();

#endif