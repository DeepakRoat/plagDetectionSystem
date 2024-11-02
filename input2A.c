#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_SIZE 100
#define THRESHOLD 0.7
#define BUFFER_SIZE 100

// Structure for each entry in the hash table
typedef struct Entry {
    char name[BUFFER_SIZE]; // Store the token
    int frequency;          // Count of occurrences
    struct Entry *next;     // Pointer to the next entry for collision handling
} Entry;

// Structure for the hash table
typedef struct Table {
    Entry **buckets; // Array of hash entries
    int current_size; // Current size of the table
    int total_tokens; // Number of unique tokens
} Table;

// Function prototypes
Table* create_table();
unsigned int generate_hash(const char *name);
void add_token(Table *table, const char *name);
int find_token_count(Table *table, const char *name);
void release_table(Table *table);
void analyze_code(const char *source, Table *table);
void resize_table(Table *table);
void display_token_frequencies(Table *table);
void read_file(const char *filename);
void standardize_token(char *name);
int is_frequent_token(const char *name);
void skip_single_line_comment(const char **source);
void skip_multi_line_comment(const char **source);

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    read_file(argv[1]);
    return 0;
}

// Create a new hash table
Table* create_table() {
    Table *table = malloc(sizeof(Table));
    table->current_size = INITIAL_SIZE;
    table->total_tokens = 0;
    table->buckets = malloc(sizeof(Entry*) * table->current_size);

    for (int i = 0; i < table->current_size; i++) {
        table->buckets[i] = NULL; // Initialize buckets to NULL
    }

    return table;
}

// Hash function to compute the index for a token
unsigned int generate_hash(const char *name) {
    unsigned int hash_value = 0;
    while (*name) {
        hash_value = (hash_value << 5) + *name++;
    }
    return hash_value;
}

// Add a token into the hash table
void add_token(Table *table, const char *name) {
    unsigned int index = generate_hash(name) % table->current_size;

    Entry *entry = table->buckets[index];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            entry->frequency++;
            return;
        }
        entry = entry->next;
    }

    // Create a new entry if the token is not found
    entry = malloc(sizeof(Entry));
    strcpy(entry->name, name);
    entry->frequency = 1;
    entry->next = table->buckets[index]; // Insert at the beginning of the linked list
    table->buckets[index] = entry;
    table->total_tokens++;

    // Check if resizing is needed
    if ((float)table->total_tokens / table->current_size > THRESHOLD) {
        resize_table(table);
    }
}

// Get the count of a token in the hash table
int find_token_count(Table *table, const char *name) {
    unsigned int index = generate_hash(name) % table->current_size;
    Entry *entry = table->buckets[index];

    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry->frequency;
        }
        entry = entry->next;
    }
    return 0; // Token not found
}

// Free the memory allocated for the hash table
void release_table(Table *table) {
    for (int i = 0; i < table->current_size; i++) {
        Entry *entry = table->buckets[i];
        while (entry) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

// Resize the hash table when the load factor exceeds the threshold
void resize_table(Table *table) {
    int old_size = table->current_size;
    int new_size = old_size * 2;
    Entry *new_buckets = malloc(sizeof(Entry) * new_size);

    for (int i = 0; i < new_size; i++) {
        new_buckets[i] = NULL; // Initialize new buckets
    }

    // Rehash old entries into new table
    for (int i = 0; i < old_size; i++) {
        Entry *entry = table->buckets[i];
        while (entry) {
            Entry *next_entry = entry->next;
            unsigned int new_index = generate_hash(entry->name) % new_size;
            entry->next = new_buckets[new_index]; // Insert at new index
            new_buckets[new_index] = entry;
            entry = next_entry;
        }
    }

    free(table->buckets); // Free old buckets
    table->buckets = new_buckets; // Point to new buckets
    table->current_size = new_size; // Update the size
}

// Standardize token by converting to lowercase
void standardize_token(char *name) {
    for (int i = 0; name[i]; i++) {
        name[i] = tolower(name[i]);
    }
}

// Check if a token is a frequent token
int is_frequent_token(const char *name) {
    const char *common_tokens[] = {
        "int", "float", "char", "void", "return", "if", "else", "for", "while", "do",
        "break", "continue", "switch", "case", "default", "include", "define", "#", 
        NULL // Sentinel value to mark the end of the list
    };

    for (int i = 0; common_tokens[i] != NULL; i++) {
        if (strcmp(name, common_tokens[i]) == 0) {
            return 1; // Token is common
        }
    }
    return 0; // Token is not common
}

// Tokenize the source code and store tokens in the hash table
void analyze_code(const char *source, Table *table) {
    char buffer[BUFFER_SIZE];
    int j = 0;
    int in_comment = 0; // Flag to track if we are inside a comment

    for (int i = 0; source[i] != '\0'; i++) {
        // Skip single-line comments
        if (source[i] == '/' && source[i + 1] == '/') {
            skip_single_line_comment(&source);
            continue; // Skip this line
        }

        // Handle multi-line comments
        if (source[i] == '/' && source[i + 1] == '*') {
            skip_multi_line_comment(&source);
            continue; // Skip processing this comment
        }

        // Process tokens only if we are not in a comment
        if (!in_comment) {
            if (isalnum(source[i]) || source[i] == '_') {
                buffer[j++] = source[i];
            } else {
                if (j > 0) {
                    buffer[j] = '\0'; // Null-terminate the string
                    standardize_token(buffer);
                    if (!is_frequent_token(buffer)) {
                        add_token(table, buffer);
                    }
                    j = 0; // Reset index
                }
                // Handle operators and punctuation
                if (!isspace(source[i])) {
                    char op[2] = {source[i], '\0'};
                    standardize_token(op); // Standardize single-character tokens
                    if (!is_frequent_token(op)) {
                        add_token(table, op);
                    }
                }
            }
        }
    }

    // Check for remaining token in buffer
    if (j > 0) {
        buffer[j] = '\0'; // Null-terminate
        standardize_token(buffer);
        if (!is_frequent_token(buffer)) {
            add_token(table, buffer);
        }
    }
}

// Skip single line comments
void skip_single_line_comment(const char **source) {
    while (**source != '\n' && **source != '\0') {
        (*source)++;
    }
}

// Skip multi-line comments
void skip_multi_line_comment(const char **source) {
    source += 2; // Skip past "/"
    while (**source != '\0') {
        if (*source == '' && *(*source + 1) == '/') {
            source += 2; // Skip past "/"
            return;
        }
        (*source)++;
    }
}

// Read and process the input file
void read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Create hash table for tokens
    Table *table = create_table();

    // Read file content into a string
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0'; // Null-terminate the string

    // Analyze code content
    analyze_code(content, table);
    free(content); // Free the content buffer
    fclose(file); // Close the file

    // Display token counts
    display_token_frequencies(table);
    release_table(table); // Free hash table
}

// Display the counts of tokens in the hash table
void display_token_frequencies(Table *table) {
    printf("Token Frequencies:\n");
    for (int i = 0; i < table->current_size; i++) {
        Entry *entry = table->buckets[i];
        while (entry) {
            printf("%s: %d\n", entry->name, entry->frequency);
            entry = entry->next;
        }
    }
}