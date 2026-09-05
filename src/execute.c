#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fork, execvp
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <fcntl.h> // open() flags O_RDONLY, O_WRONLY, dup2
#include <string.h> // strcmp
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
        // for i/o redirection
        char* input_file = NULL;
        char* output_file = NULL;
        int redirect_index = -1;

        // scanning for input/output redirection in the command
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "<") == 0)
            {
                input_file = args[i + 1];
                if (redirect_index == -1)
                {
                    redirect_index = i;
                }
            }
            else if (strcmp(args[i], ">") == 0)
            {
                output_file = args[i + 1];
                if (redirect_index == -1)
                {
                    redirect_index = i;
                }
            }
        }

        if (redirect_index != -1)
        {
            args[redirect_index] = NULL;
        }

        if (input_file != NULL)
        {
            // opening the file in read only mode using open()
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0)
            {
                perror("input redirection: open failed");
                exit(EXIT_FAILURE);
            }
            // dup2 copies the file descriptor fd_in into STDIN_FILENO (file descriptor 0) so that it refers to the file descriptor number of fd_in instead if the terminal 
            if (dup2(fd_in, STDIN_FILENO) < 0)
            {
                perror("input redirection: dup2 failed");
                exit(EXIT_FAILURE);
            }
            close(fd_in); //the original file descriptor is closed
        }

        if (output_file != NULL)
        {
            // file is opened for writing, created if it doesn't exist and truncated if it does
            // 0644 gives standard read/write permissions to the owner
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            if (fd_out < 0)
            {
                perror("output redirection: open failed");
                exit(EXIT_FAILURE);
            }
            // dup2 copies the file descriptor fd_in into STDIN_FILENO (file descriptor 1) so that it fd 0 points to the output file
            if (dup2(fd_out, STDOUT_FILENO) < 0)
            {
                perror("output redirection: dup2 failed");
                exit(EXIT_FAILURE);
            }
            close(fd_out);
        }

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