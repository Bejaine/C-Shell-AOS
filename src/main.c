#include <stdio.h>
#include <stdlib.h>
#include "../include/prompt.h"
#include "../include/input.h"

int main()
{

    // setting shell home directory
    init_shell_home();

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

        // tokenizing input
        char* args[MAX_ARGS];
        int arg_count = tokenize_input(input, args);

        // debug: parsing verification
        if (arg_count > 0)
        {
            printf("Debug: Parsed Tokens\n");
            for (int i = 0; i < arg_count; i++)
            {
                printf("args[%d]: '%s'\n", i, args[i]);
            }
            printf("\n");
        }

        // freeing the allocated memory before the next loop iteration
        free(input);
    }
    
    return 0;
}