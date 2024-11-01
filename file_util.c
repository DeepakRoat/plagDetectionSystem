#include <stdio.h>
#include <stdlib.h>

char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END); // Move to the end of the file
    long length = ftell(file); // Get the length of the file
    fseek(file, 0, SEEK_SET); // Move back to the start of the file

    char *content = malloc(length + 1); // Allocate memory for the content
    fread(content, 1, length, file); // Read the file into content
    content[length] = '\0'; // Null-terminate the string

    fclose(file);
    return content;
}
