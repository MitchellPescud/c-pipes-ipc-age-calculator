#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <stdio.h>
#include <time.h>
#include <ctype.h>

// Function to calculate age from DOB
int calculate_age(const char *dob);
// Function to validate DOB format (DD-MM-YYYY)
int validate_dob(const char *dob_str);

#endif /* DATE_UTILS_H */