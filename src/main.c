#include <stdio.h>
#include "prompt.h"

int main()
{

    // setting shell home directory
    init_shell_home();

    // displaying the prompt
    display_prompt();

    return 0;
}