#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strtok
#include "../include/input.h"

#define MAX_INPUT_SIZE 4096

// reading a line of input from standard input and storing them in a buffer
char* read_input()
{
    // buffer memory for storing the user's input
    char* buffer = malloc(MAX_INPUT_SIZE);
    if (buffer == NULL)
    {
        perror("malloc failed for input buffer allocation");
        exit(EXIT_FAILURE);
    }

    // fgets to read until a newline or EOF
    // if it returns NULL, the user pressed Ctrl+D (EOF)
    if (fgets(buffer, MAX_INPUT_SIZE, stdin) == NULL)
    {
        free(buffer);
        return NULL; 
    }
    
    return buffer;
}

// parsing the input string into an array of tokens and to return the number of tokens obtained
int tokenize_input(char* input, char** args)
{
    int count = 0;
    
    // first call to strtok passing the string to be passed and delimiters
    // strtok needs a string of delimiters and '\t' handles tabs, ' ' handles spaces, '\n' removes the newline from fgets
    char* token = strtok(input, " \t\n");
    
    while (token != NULL)
    {
        args[count] = token;
        count++;
        // preventing array overflow by leaving 1 spot for the NULL terminator
        if (count >= MAX_ARGS - 1)
        {
            break;
        }
        // pass NULL to strtok to keep parsing the same string in subsequent calls to strtok
        token = strtok(NULL, " \t\n");
    }
    
    // null terminating the array of arguments
    args[count] = NULL; 
    
    return count;
}