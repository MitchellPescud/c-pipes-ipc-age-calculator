#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "file_utils.h"

int main() {
    // Create two pipes
    int pipe_fd1[2], pipe_fd2[2]; //Declare two pipe file descriptor arrays
    if (pipe(pipe_fd1) != 0 || pipe(pipe_fd2) != 0) { //Creates two pipes and exits if not successfule
        perror("Failed to create pipes");
        exit(EXIT_FAILURE); //Terminate program with failure status
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) { //If an error occurs PID returns 1
        perror("Failed to fork");
        exit(EXIT_FAILURE); //Terminate program with failure status
    }

    if (pid == 0) {  // If the current process is a child process do the following
        // Close unused pipe ends
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);
        // Call child process function with appropriate file descriptors
        child_process(pipe_fd1[0], pipe_fd2[1]);
        exit(EXIT_SUCCESS); //Terminate Program with success status
    } else {  // Parent process
        // Close unused pipe ends
        close(pipe_fd1[0]);
        close(pipe_fd2[1]);
        // Open current directory
        DIR *dir = opendir(".");
        if (!dir) { //If current directory cannot be opened terminate program with failure status
            perror("Failed to open directory");
            exit(EXIT_FAILURE);
        }
        // Call parent process function with appropriate file descriptors and directory pointer
        parent_process(pipe_fd2[0], pipe_fd1[1], dir);
        closedir(dir); //Closes directory

        // Wait for child process to complete
        wait(NULL);
    }

    return 0;
}
