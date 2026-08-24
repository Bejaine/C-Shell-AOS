#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getcwd, getuid, gethostname
#include <pwd.h> // getpwuid
#include <sys/types.h> // getuid, getpwuid
#include <limits.h>

#define MAX_BUFFER 1024

// global storage for the directory from which the shell was invoked
static char home_dir[PATH_MAX] = "";

// initialising the directory from which the shell was invoked as the home directory using getcwd
void init_shell_home()
{
    if (getcwd(home_dir, sizeof(home_dir)) == NULL)
    {
        perror("getcwd failed during initialization of home directory");
        exit(EXIT_FAILURE);
    }
}

// displaying the shell prompt after getting the system username
void display_prompt(void)
{
    // getting system username
    // getuid() fetches the user's numeric ID
    // getpwuid() queries the passwd structure using the user's numeric ID
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    const char *username = NULL;
    if (pw != NULL)
    {
        username = pw->pw_name;
    }
    else
    {
        username = "unknown";
    }

    // getting system / host name
    char hostname[MAX_BUFFER];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        strncpy(hostname, "unknown", sizeof(hostname));
    }
    // Ensure null termination
    hostname[sizeof(hostname) - 1] = '\0';

    // getting current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd failed to obtain current working directory");
        return;
    }

    // transforming current directory relative to the shell's home directory
    char relative_dir[PATH_MAX];
    size_t home_len = strlen(home_dir);

    // if the current directory is exactly the shell home directory
    if (strcmp(cwd, home_dir) == 0)
    {
        strcpy(relative_dir, "~");
    }
    // if the current directory is a subdirectory inside the shell home directory
    else if (strncmp(cwd, home_dir, home_len) == 0 && (cwd[home_len] == '/' || home_len == 1))
    {
        snprintf(relative_dir, sizeof(relative_dir), "~%s", cwd + home_len); // using pointer arithmetic to get relative directory
    }
    // if the current directory is outside of home directory then we must show full absolute path
    else
    {
        strncpy(relative_dir, cwd, sizeof(relative_dir));
        relative_dir[sizeof(relative_dir) - 1] = '\0';
    }

    // printing the prompt in the format <username@system_name:curr_dir>
    printf("<%s@%s:%s> ", username, hostname, relative_dir);
    
    fflush(stdout); // forcing stdout buffer to immediately display without waiting for input
}