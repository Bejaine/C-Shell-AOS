#ifndef INPUT_H
#define INPUT_H

#define MAX_ARGS 100 // maximum number of arguments that a user can type

// reading a line of input from standard input
char* read_input(void);

// parsing the input string into an array of tokens and to return the number of tokens obtained
int tokenize_input(char* input, char** args);

#endif