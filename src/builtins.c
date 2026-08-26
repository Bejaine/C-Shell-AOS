#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getcwd
#include <limits.h> // PATH_MAX
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

// obtains the present working directory
void execute_pwd()
{
    char cwd[PATH_MAX];
    
    // getcwd fetches the absolute path 
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd failed during execution of pwd command");
    }
}