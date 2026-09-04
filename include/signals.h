#ifndef SIGNALS_H
#define SIGNALS_H

// signal handler for the shell to catch the SIGCHLD signal from the os for background process cleanup
void setup_background_cleanup();

#endif