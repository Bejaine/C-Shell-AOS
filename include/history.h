#ifndef HISTORY_H
#define HISTORY_H

// loads history from the file ~/.history when the shell boots
void init_history();

// adding the previous command to the history array and saving it to the file
void add_history(const char* raw_cmd);

// for exectuing the built-in command history to display the last 10 commands
void execute_history();

#endif