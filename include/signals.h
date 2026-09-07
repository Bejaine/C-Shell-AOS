#ifndef SIGNALS_H
#define SIGNALS_H

extern pid_t current_foreground_pid;

// signal handler for the shell to catch the SIGCHLD signal from the os for background process cleanup
void setup_background_cleanup();

// sets up the signal handler to catch Ctrl+C (SIGINT)
void setup_sigint_handler(void);

#endif