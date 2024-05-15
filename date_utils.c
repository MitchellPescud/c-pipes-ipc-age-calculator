#include "date_utils.h"

// Function to calculate age from DOB
int calculate_age(const char *dob) {
    // Define structures to handle dates
    struct tm birth_date = {0};
    struct tm date_today = {0};
    // Get current time in UTC format
    time_t current_time = time(NULL);
    // Convert current time to local time so it's accurate to whatever timezone the user is in
    localtime_r(&current_time, &date_today); // Use localtime_r to store the result in date_today
    // Parse DOB string and populate birth_date structure
    sscanf(dob, "%d-%d-%d", &birth_date.tm_mday, &birth_date.tm_mon, &birth_date.tm_year);
    // Adjust year and month to match tm structure
    birth_date.tm_year -= 1900;  // Year since 1900 cause unix systems are dated and were created when the Y2k problem was years away
    birth_date.tm_mon -= 1;       // Adjust Month as it is represented as 0-11 not 1-12 like humans read
    
    // Calculate age
    int age = date_today.tm_year - birth_date.tm_year;
    // Adjust age if birth date hasn't occurred yet this year
    if (date_today.tm_mon < birth_date.tm_mon) { //Check if birth month is greater than current month
        if (date_today.tm_mon == birth_date.tm_mon && date_today.tm_mday < birth_date.tm_mday) { //Check if birth month is current month and date is greater than current date
            age--; //Decrease age by 1
        }
    }

    return age;
}

// Function to validate DOB format (DD-MM-YYYY)
int validate_dob(const char *dob_str) {
    int day, month, year;
    // Attempt to parse DOB string
    if (sscanf(dob_str, "%d-%d-%d", &day, &month, &year) != 3) {
        return 0; // Return 0 for invalid date format
    }
    // Check for valid day, month, and year
    if (day < 1 || day > 31 || month < 1 || month > 12 || year <= 0 || year >= 9999) {
        return 0; // Return 0 for invalid date components
    }
    // Check for unwanted characters in the input string
    for (const char *character_ptr = dob_str; *character_ptr != '\0'; character_ptr++) {
        if (!isdigit(*character_ptr) && *character_ptr != '-') {
            return 0; // Return 0 if anything but a digit or - is detected
        }
    }
    return 1; // Return 1 for DOB
}