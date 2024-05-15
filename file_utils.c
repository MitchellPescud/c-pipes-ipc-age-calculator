#include "file_utils.h"

#define BUFFER_SIZE 1024  // Buffer size can be increased if required

// Child process function to read Name and DOB from USP files
void child_process(int read_fd, int write_fd) {
    char filename[BUFFER_SIZE];
    int num_bytes, file_fd;

    // Read file names from pipe
    while ((num_bytes = read(read_fd, filename, BUFFER_SIZE)) > 0) {
        filename[num_bytes] = '\0';  // Ensure null termination
        printf("Processing file: %s\n", filename);  // Debug Use

        // Open the file for reading
        file_fd = open(filename, O_RDONLY); // Open file from file descriptor as read only
        if (file_fd == -1) { // Check if open function failed
            perror("Failed to open file");
            continue; // Skip rest of loop iteration if file failed to open
        }

        // Read content of the file
        char buffer[BUFFER_SIZE]; // Store contents as a char array of buffer size
        int bytes_read = read(file_fd, buffer, BUFFER_SIZE); // Read up to the buffer size number of bytes, Only stores actual number of bytes read
        close(file_fd); // Close file from file descriptor
        if (bytes_read <= 0) continue; // If no bytes successfully read skip rest of loop iteration

        buffer[bytes_read] = '\0'; // Ensure null termination of buffer
        // Extract name and DOB from file content
        char *name = strtok(buffer, "\n"); // Extracts first line of buffer till delimiter of \n
        char *dob = strtok(NULL, "\n"); // Extracts second line of buffer from where it left of till same delimiter
        // Validate DOB
        if (!validate_dob(dob)) { //Do the following if validate_dob returning 0 due to invalid DOB
            fprintf(stderr, "Skipping file %s due to invalid DOB.\n", filename);
            continue; // Skip processing for files with invalid DOB
        }
        printf("Name From %s: %s\n", filename, name); // Debug Use
        printf("DOB From %s: %s\n", filename, dob); // Debug Use
        // Calculate age from DOB
        int age = calculate_age(dob);

        // Prepare result string with name and age
        char result[BUFFER_SIZE];
        snprintf(result, BUFFER_SIZE, "%s:%d\n", name, age);
        
        // Write result to the write pipe
        write(write_fd, result, strlen(result));
    }
    // Close file descriptors
    close(read_fd);
    close(write_fd);
}

// Parent process function
void parent_process(int read_fd, int write_fd, DIR *dir) {
    struct dirent *entry;
    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file name ends with ".usp"
        if (strcmp(entry->d_name + strlen(entry->d_name) - 4, ".usp") == 0) {
            // Get length of file name
            int len = strlen(entry->d_name) + 1;
            printf("Sending file: %s\n", entry->d_name);  // Debug Use
            // Write file name and descriptor to pipe
            write(write_fd, entry->d_name, len);
            // Add a small delay to ensure proper synchronization
            usleep(100000);
        }
    }
    // Close write pipe to signal end of file names to child
    close(write_fd);


    // 0644 Sets file to read and write to owner but read only for everyone else
    // Open result.txt as write only, if it doesn't exist create it and if it does append to it
    int result_fd = open("result.txt", O_WRONLY | O_CREAT | O_APPEND, 0644); 
    if (result_fd == -1) { // If open failed print error and terminate program
        perror("Failed to open result.txt");
        exit(EXIT_FAILURE);
    }

    char result[BUFFER_SIZE];
    int num_bytes;
    // Read results from the read pipe and write to result.txt from the result array
    while ((num_bytes = read(read_fd, result, BUFFER_SIZE)) > 0) {
        write(result_fd, result, num_bytes);
    }
    // Close file descriptors
    close(read_fd);
    close(result_fd);
}
