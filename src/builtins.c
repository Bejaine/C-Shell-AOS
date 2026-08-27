#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getcwd
#include <limits.h> // PATH_MAX
#include <string.h> // strcmp
#include "../include/builtins.h"
#include "../include/prompt.h"

// global storage strictly for this file to remember the previous directory
static char prev_dir[PATH_MAX] = "";

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

void execute_cd(char** args)
{
    // checking for too many arguments
    if (args[1] != NULL && args[2] != NULL)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    char* target_dir = NULL;
    const char* home_dir = get_shell_home();

    // determining the destination
    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        // 'cd' with no arguments or 'cd ~' goes to the shell home directory
        target_dir = (char*)home_dir;
    }
    else if (strcmp(args[1], "-") == 0)
    {
        // 'cd -' goes to the previous directory
        if (strlen(prev_dir) == 0)
        {
            fprintf(stderr, "cd: no previous directory\n");
            return;
        }
        target_dir = prev_dir;
    }
    else
    {
        // 'cd ..' or 'cd [path]' goes to the directory of the argument
        // we don't need a special case for 'cd ..' since OS understands '..' automatically 
        target_dir = args[1];
    }

    // saving the current directory before moving directory
    char current_cwd[PATH_MAX];
    if (getcwd(current_cwd, sizeof(current_cwd)) == NULL)
    {
        perror("cd: getcwd failed");
        return;
    }

    // call the os function chdir to change working directory
    if (chdir(target_dir) != 0)
    {
        // when the directory doesn't exist or when permissions are denied
        perror("cd: chdir cannot execute");
    }
    else
    {
        // once chdir moves current working directory, we update previous directory
        strncpy(prev_dir, current_cwd, sizeof(prev_dir));
        
        // printing the destination directory for 'cd -'
        if (args[1] != NULL && strcmp(args[1], "-") == 0)
        {
            char new_cwd[PATH_MAX];
            if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
            {
                printf("%s\n", new_cwd);
            }
        }
    }
}