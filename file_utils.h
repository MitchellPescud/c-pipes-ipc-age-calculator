#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include "date_utils.h"

// Child process function to read Name and DOB from USP files
void child_process(int read_fd, int write_fd);
// Parent process function
void parent_process(int read_fd, int write_fd, DIR *dir);

#endif /* FILE_UTILS_H */
