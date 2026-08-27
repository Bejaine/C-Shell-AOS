#ifndef PROMPT_H
#define PROMPT_H

// initialising the directory from which the shell was invoked as the home directory
void init_shell_home();

// displaying the shell prompt
void display_prompt();

// getting the shell's home directory path from prompt.c
const char* get_shell_home(void);

#endif