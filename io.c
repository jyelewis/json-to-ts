#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

#define BUF_SIZE 1024

char* load_from_stdin() {
    char buffer[BUF_SIZE];
    size_t input_size = 0;
    char *input = malloc(sizeof(char) * BUF_SIZE);
    if (input == NULL) {
        perror("Failed to allocate buffer");
        exit(1);
    }

    size_t bytes_read_into_buffer;
    while ((bytes_read_into_buffer = fread(buffer, sizeof(char), BUF_SIZE, stdin))) {
        // hold a reference so we can free on a failure to expand
        char *old_input = input;
        size_t old_input_size = input_size;

        // expand
        input_size += bytes_read_into_buffer;
        input = realloc(input, input_size); // expand our input memory to fix newly read bytes
        if (input == NULL) {
            perror("Failed to reallocate input buffer");
            free(old_input);
            exit(2);
        }

        // copy our newly read buffer into our input data
        strncpy(&input[old_input_size], buffer, bytes_read_into_buffer);
    }

    return input;
}