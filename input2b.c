#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_TABLE_SIZE 100
#define LOAD_FACTOR_THRESHOLD 0.7
#define TOKEN_BUFFER_SIZE 100

// Structure for hash table entry
typedef struct HashEntry {
    char token[TOKEN_BUFFER_SIZE]; // Store the token
    int count;       // Count of occurrences
    struct HashEntry *next; // Pointer to the next entry in case of collision
} HashEntry;

// Structure for the hash table
typedef struct HashTable {
    HashEntry **entries; // Array of hash entries
    int size; // Current size of the table
    int count; // Number of unique tokens
} HashTable;

// Function prototypes
HashTable* create_hash_table();
unsigned int hash(const char *token);
void insert_token(HashTable *table, const char *token);
int get_token_count(HashTable *table, const char *token);
void free_hash_table(HashTable *table);
void tokenize_code(const char *content, HashTable *table);
void rehash(HashTable *table);
void print_token_counts(HashTable *table);
void process_file(const char *filename);
void normalize_token(char *token);
int is_common_token(const char *token);
void skip_single_line_comment(const char **content);
void skip_multi_line_comment(const char **content);

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    process_file(argv[1]);
    return 0;
}

// Create a new hash table
HashTable* create_hash_table() {
    HashTable *table = malloc(sizeof(HashTable));
    table->size = INITIAL_TABLE_SIZE;
    table->count = 0;
    table->entries = malloc(sizeof(HashEntry*) * table->size);

    for (int i = 0; i < table->size; i++) {
        table->entries[i] = NULL; // Initialize entries to NULL
    }

    return table;
}

// Hash function to compute the index for a token
unsigned int hash(const char *token) {
    unsigned int hash_value = 0;
    while (*token) {
        hash_value = (hash_value << 5) + *token++;
    }
    return hash_value;
}

// Insert a token into the hash table
void insert_token(HashTable *table, const char *token) {
    unsigned int index = hash(token) % table->size;

    HashEntry *entry = table->entries[index];
    while (entry) {
        if (strcmp(entry->token, token) == 0) {
            entry->count++;
            return;
        }
        entry = entry->next;
    }

    // Create a new entry if the token is not found
    entry = malloc(sizeof(HashEntry));
    strcpy(entry->token, token);
    entry->count = 1;
    entry->next = table->entries[index];
    table->entries[index] = entry;
    table->count++;

    // Check if rehashing is needed
    if ((float)table->count / table->size > LOAD_FACTOR_THRESHOLD) {
        rehash(table);
    }
}

// Get the count of a token in the hash table
int get_token_count(HashTable *table, const char *token) {
    unsigned int index = hash(token) % table->size;
    HashEntry *entry = table->entries[index];

    while (entry) {
        if (strcmp(entry->token, token) == 0) {
            return entry->count;
        }
        entry = entry->next;
    }
    return 0; // Token not found
}

// Free the memory allocated for the hash table
void free_hash_table(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        HashEntry *entry = table->entries[i];
        while (entry) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(table->entries);
    free(table);
}

// Rehash the hash table when load factor exceeds threshold
void rehash(HashTable *table) {
    int old_size = table->size;
    int new_size = old_size * 2;
    HashEntry *new_entries = malloc(sizeof(HashEntry) * new_size);

    for (int i = 0; i < new_size; i++) {
        new_entries[i] = NULL; // Initialize new entries
    }

    // Rehash old entries into new table
    for (int i = 0; i < old_size; i++) {
        HashEntry *entry = table->entries[i];
        while (entry) {
            HashEntry *next_entry = entry->next;
            unsigned int new_index = hash(entry->token) % new_size;
            entry->next = new_entries[new_index]; // Insert at new index
            new_entries[new_index] = entry;
            entry = next_entry;
        }
    }

    free(table->entries); // Free old entries
    table->entries = new_entries; // Point to new entries
    table->size = new_size; // Update the size
}

// Normalize token by converting to lowercase
void normalize_token(char *token) {
    for (int i = 0; token[i]; i++) {
        token[i] = tolower(token[i]);
    }
}

// Check if a token is a common token
int is_common_token(const char *token) {
    const char *common_tokens[] = {
        "int", "float", "char", "void", "return", "if", "else", "for", "while", "do",
        "break", "continue", "switch", "case", "default", "include", "define", "#", 
        NULL // Sentinel value to mark the end of the list
    };

    for (int i = 0; common_tokens[i] != NULL; i++) {
        if (strcmp(token, common_tokens[i]) == 0) {
            return 1; // Token is common
        }
    }
    return 0; // Token is not common
}

// Tokenize the code content and store tokens in hash table
void tokenize_code(const char *content, HashTable *table) {
    char buffer[TOKEN_BUFFER_SIZE];
    int j = 0;
    int in_comment = 0; // Flag to track if we are inside a comment

    for (int i = 0; content[i] != '\0'; i++) {
        // Skip single-line comments
        if (content[i] == '/' && content[i + 1] == '/') {
            skip_single_line_comment(&content);
            continue; // Skip this line
        }

        // Handle multi-line comments
        if (content[i] == '/' && content[i + 1] == '*') {
            skip_multi_line_comment(&content);
            continue; // Skip processing this comment
        }

        // Process tokens only if we are not in a comment
        if (!in_comment) {
            if (isalnum(content[i]) || content[i] == '_') {
                buffer[j++] = content[i];
            } else {
                if (j > 0) {
                    buffer[j] = '\0'; // Null-terminate the string
                    normalize_token(buffer);
                    if (!is_common_token(buffer)) {
                        insert_token(table, buffer);
                    }
                    j = 0; // Reset index
                }
                // Handle operators and punctuation
                if (!isspace(content[i])) {
                    char op[2] = {content[i], '\0'};
                    normalize_token(op); // Normalize single-character tokens
                    if (!is_common_token(op)) {
                        insert_token(table, op);
                    }
                }
            }
        }
    }

    // Check for remaining token in buffer
    if (j > 0) {
        buffer[j] = '\0'; // Null-terminate
        normalize_token(buffer);
        if (!is_common_token(buffer)) {
            insert_token(table, buffer);
        }
    }
}

// Skip single line comments
void skip_single_line_comment(const char **content) {
    while (**content != '\n' && **content != '\0') {
        (*content)++;
    }
}

// Skip multi-line comments
void skip_multi_line_comment(const char **content) {
    content += 2; // Skip past "/"
    while (**content != '\0') {
        if (*content == '' && *(*content + 1) == '/') {
            content += 2; // Skip past "/"
            return;
        }
        (*content)++;
    }
}

// Process the input file
void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Create hash table for tokens
    HashTable *table = create_hash_table();

    // Read file content into a string
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(file_size + 1);
    fread(content, 1, file_size, file);
    content[file_size] = '\0'; // Null-terminate the string

    // Tokenize code content
    tokenize_code(content, table);
    free(content); // Free the content buffer
    fclose(file); // Close the file

    // Print token counts
    print_token_counts(table);
    free_hash_table(table); // Free hash table
}

// Print the counts of tokens in the hash table
void print_token_counts(HashTable *table) {
    printf("Token Counts:\n");
    for (int i = 0; i < table->size; i++) {
        HashEntry *entry = table->entries[i];
        while (entry) {
            printf("%s: %d\n", entry->token, entry->count);
            entry = entry->next;
        }
    }
}
