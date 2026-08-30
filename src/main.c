#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/prompt.h"
#include "../include/input.h"
#include "../include/builtins.h"
#include "../include/history.h"
#include "../include/execute.h"

int main()
{

    // setting shell home directory
    init_shell_home();

    // loading history from the history
    init_history();

    // displaying the prompt in a loop
    while(1)
    {
        display_prompt();

        // reading input
        char* input = read_input();
        // if input is NULL, user pressed Ctrl+D
        if (input == NULL)
        {
            printf("\nExiting shell...\n");
            break; 
        }

        // obtaining the input command before tokenising for history command
        char raw_input[4096];
        strncpy(raw_input, input, sizeof(raw_input));

        // tokenizing input
        char* args[MAX_ARGS];
        int arg_count = tokenize_input(input, args);

        // debug: parsing verification
        // if (arg_count > 0)
        // {
        //     printf("Debug: Parsed Tokens\n");
        //     for (int i = 0; i < arg_count; i++)
        //     {
        //         printf("args[%d]: '%s'\n", i, args[i]);
        //     }
        //     printf("\n");
        // }

        // if the input is an empty line, starting the loop over
        if (arg_count == 0)
        {
            free(input);
            continue;
        }

        // adding the command to history after checking if it was an empty line
        add_history(raw_input);

        // checking for built-in commands
        if (strcmp(args[0], "echo") == 0)
        {
            execute_echo(args);
        } 
        else if (strcmp(args[0], "pwd") == 0)
        {
            execute_pwd();
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            execute_cd(args);
        }
        else if (strcmp(args[0], "history") == 0)
        {
            execute_history();
        }
        else
        {
            int is_background = 0;
            int last_arg_index = arg_count - 1;

            // checking if the last token is & to assign background process and ensuring execvp doesn't try to execute it
            if (last_arg_index >= 0 && strcmp(args[last_arg_index], "&") == 0)
            {
                is_background = 1;
                args[last_arg_index] = NULL; 
            }

            // executing the external command with the foreground/background flag
            execute_external_command(args, is_background); 
        }

        // freeing the allocated memory before the next loop iteration
        free(input);
    }
    
    return 0;
}