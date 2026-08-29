#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // PATH_MAX
#include "../include/history.h"
#include "../include/prompt.h" // get_shell_home()

#define MAX_HISTORY 20
#define MAX_CMD_LEN 4096

// global storage to store 20 past commands
static char history_list[MAX_HISTORY][MAX_CMD_LEN];
static int history_count = 0;

// function to construct the absolute path to the .history file
static void get_history_file_path(char* path_buffer)
{
    snprintf(path_buffer, PATH_MAX, "%s/.history", get_shell_home());
}

// function to save the current history_list array to the file ~/.history
static void save_history()
{
    char filepath[PATH_MAX];
    get_history_file_path(filepath);

    // opening the file in write mode to overwrite it with the current history_list array
    FILE *file = fopen(filepath, "w");
    if (file == NULL)
    {
        perror("history: could not open file for writing");
        return;
    }

    for (int i = 0; i < history_count; i++)
    {
        fprintf(file, "%s", history_list[i]);
    }
    fclose(file);
}

// loading history from the file ~/.history when the shell boots
void init_history()
{
    char filepath[PATH_MAX];
    get_history_file_path(filepath);

    // opening the file in read only mode
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        return;
    }

    char buffer[MAX_CMD_LEN];
    // reading line by line using newline until EOF
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // making sure only 20 past commands from .history are initialised into the history_list array
        if (history_count < MAX_HISTORY)
        {
            strncpy(history_list[history_count], buffer, MAX_CMD_LEN);
            history_count++;
        }
    }
    fclose(file);
}

// adding the previous command to the history_list array and saving it to the file
void add_history(const char *raw_cmd)
{
    // preventing storage of consecutive duplicates
    if (history_count > 0 && strcmp(history_list[history_count - 1], raw_cmd) == 0)
    {
        return;
    }

    // adding the command to the array history_list
    // if the array contains 20 commands, we shift all elements left and overwrite the oldest command
    // else, we just add it to the end
    if (history_count == MAX_HISTORY)
    {
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            strncpy(history_list[i - 1], history_list[i], MAX_CMD_LEN);
        }
        strncpy(history_list[MAX_HISTORY - 1], raw_cmd, MAX_CMD_LEN);
    }
    else
    {
        strncpy(history_list[history_count], raw_cmd, MAX_CMD_LEN);
        history_count++;
    }

    // we update the ~/.history file with the updated set of commands from the array
    save_history();
}

// printing the last 10 commands when the history command is invoked
void execute_history()
{
    int start_index = 0;
    if (history_count > 10)
    {
        start_index = history_count - 10;
    }

    for (int i = start_index; i < history_count; i++)
    {
        // raw_cmd includes the newline from fgets so a separate newline print is not required
        printf("%s", history_list[i]);
    }
}