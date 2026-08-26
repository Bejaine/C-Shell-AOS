#include <stdio.h>
#include "../include/builtins.h"

void execute_echo(char** args)
{
    // starting at index 1 to skip the "echo" command
    int i = 1;
    
    // loopoing through all remaining tokens
    while (args[i] != NULL)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
        {
            printf(" ");
        }
        i++;
    }
    printf("\n");
}