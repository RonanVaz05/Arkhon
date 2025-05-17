#include "../include/utils.h"
#include <string.h>
#include <ctype.h> // For isspace

// Removes leading and trailing whitespace from a string, in-place.
char *str_trim(char *s) {
    char *start, *end;

    // Trim leading space
    for (start = s; *start && isspace((unsigned char)*start); ++start);

    // Trim trailing space
    if (*start == 0) { // All spaces?
        *s = '\0';
        return s;
    }

    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    // Shift the trimmed string to the beginning if there was leading space
    if (s != start) {
        memmove(s, start, (end - start) + 2); // +1 for char, +1 for null terminator
    }

    return s;
} 