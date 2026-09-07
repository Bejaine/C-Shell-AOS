#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // SIGCHLD, struct sigaction, sigaction, sigemptysetm, SA_RESTART
#include <sys/wait.h> // waitpid, WNOHANG
#include <unistd.h>
#include "../include/signals.h"
#include "../include/prompt.h"

pid_t current_foreground_pid = -1;

// function that runs when the SIGCHLD signal is received
static void sigchld_handler(int sig)
{
    (void) sig;

    pid_t pid;
    int status;

    // parent waits for any child process to complete/terminate and returns immediately (non-blocking) if no child has actually exited
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("CHILD PROCESS %d TERMINATED\n", pid);
    }
    return;
}

void setup_background_cleanup()
{
    /*
    struct sigaction
    {
        void (*sa_handler) (int);
        void (*sa_sigaction) (int, siginfo_t*, void*);
        sigset_t sa_mask;
        int sa_flags;
        void (*sa_restorer) (void);
    }
    */
    struct sigaction sa;
    
    // pointing the action to the handler function
    sa.sa_handler = sigchld_handler;
    
    // int sigemptyset(sigset_t* set) initialises the signal given by set to empty with all signals excluded from the set
    // clearing out the signal mask since it specifies a mask of signals which should be blocked during execution of the signal handler
    sigemptyset(&sa.sa_mask);
    
    // if a background process finishes while the shell is blocked waiting for user input (fgets), the signal will interrupt fgets
    // SA_RESTART flag tells the OS to automatically restart the interrupted fgets call.
    sa.sa_flags = SA_RESTART;

    // making the OS catch SIGCHLD using our configuration
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("error setting up SIGCHLD handler");
        exit(EXIT_FAILURE);
    }
}

// function that runs when Ctrl+C (SIGINT) is pressed
static void sigint_handler(int sig)
{
    (void)sig; // for the unused parameter warning
    
    // printing a newline to move past whatever the user was typing
    printf("\n");
    
    if (current_foreground_pid == -1)
    {
        display_prompt();
        fflush(stdout); // forcing the prompt to print immediately
    }
}

void setup_sigint_handler(void)
{
    struct sigaction sa;
    
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask); // ensures that we aren't explicitly blocking any additional signals
    
    // SA_RESTART ensures that if blocking system call commands like fgets is interrupted by Ctrl+C, it resumes waiting for input
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction: error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }
}