#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // execvp
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include "../include/execute.h"

void execute_external_command(char** args, int is_background)
{
    // duplicating the current parent shell process using fork
    // process id using linux type pid_t giving a signed integer
    pid_t pid = fork();

    // if fork fails due to process limits or memory limits, pid < 0
    // else if child is created, then pid = 0 and child process is executing
    // else if pid > 0, we are in the parent process
    if (pid < 0)
    {
        perror("fork: failed");
    } 
    else if (pid == 0)
    {
        // execvp replaces the child's memory with the new program,
        // thus executing the command's (arg[0]'s) program instead of the shell
        // and stopping this program here
        if (execvp(args[0], args) == -1)
        {
            // if execvp returns, then the program failed to run
            perror(args[0]); 
            // to prevent the child from running another shell after failure, the child process must be stopped
            exit(EXIT_FAILURE); 
        }
    } 
    else
    {
        if (is_background)
        {
            // parent shell doesn't wait for a background process
            printf("Background Process PID: %d\n", pid);
        }
        else
        {
            // parent waits for any of it's child processes with the same process group id as the calling process, using waitpid until a state change like termination, stopping or resumption of the child process occurs
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("waitpid: failed");
            }
        }
    }
}