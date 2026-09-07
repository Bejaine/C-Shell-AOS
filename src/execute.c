#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fork, execvp, pipe
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <fcntl.h> // open() flags O_RDONLY, O_WRONLY, dup2
#include <string.h> // strcmp
#include "../include/execute.h"

void execute_external_command(char** args, int is_background)
{

    // checking for pipe
    int pipe_index = -1;
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            pipe_index = i;
            break;
        }
    }

    // piping
    if (pipe_index != -1)
    {
        // splitting the command into two different commands for piping
        args[pipe_index] = NULL;
        char **left_cmd = args;
        char **right_cmd = &args[pipe_index + 1];

        // int pipe(int pipefd[2])
        // the array pipefd[2] is used to return two file descriptors referring to the ends of the pipe, with pipefd[0] referring to the read end of the pipe and with pipefd[1] referring to the write end of the pipe
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
            return;
        }

        // forking the left (read) command as a child
        pid_t pid_read = fork();
        if (pid_read < 0)
        {
            perror("pipe: read fork failed");
            return;
        }

        if (pid_read == 0)
        {
            close(pipefd[0]); // read child process doesn't read from the pipe, so we close the read end file desriptor
            dup2(pipefd[1], STDOUT_FILENO); // rerouting the standard output from the terminal to the file descriptor of thw write command
            close(pipefd[1]); // closing the write file descriptor

            // execvp runs the left (read) command
            if (execvp(left_cmd[0], left_cmd) == -1)
            {
                perror(left_cmd[0]);
                exit(EXIT_FAILURE);
            }
        }

        // forking the left (write) command as a child
        pid_t pid_write = fork();
        if (pid_write < 0)
        {
            perror("pipe: write fork failed");
            return;
        }

        if (pid_write == 0)
        {
            close(pipefd[1]); // write child process doesn't write to the pipe, so we close the write end file desriptor
            dup2(pipefd[0], STDIN_FILENO); // rerouting the standard output from the terminal to the file descriptor of the read command
            close(pipefd[0]); // closing the read file descriptor

            if (execvp(right_cmd[0], right_cmd) == -1)
            {
                perror(right_cmd[0]);
                exit(EXIT_FAILURE);
            }
        }

        // parent shell closes both ends of the pipe so the child processes don't wait for more data
        close(pipefd[0]);
        close(pipefd[1]);

        if (is_background)
        {
            printf("Background Pipeline PIDs: %d, %d\n", pid_read, pid_write);
        }
        else
        {
            // waiting for both children to finish
            waitpid(pid_read, NULL, 0);
            waitpid(pid_write, NULL, 0);
        }
        
        // returning early when the pipeline execution is finished
        return; 
    }

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