#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// Create a new hash table
HashTable* create_hash_table() {
    HashTable *table = malloc(sizeof(HashTable));
    table->entries = malloc(sizeof(HashEntry*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->entries[i] = NULL; // Initialize all entries to NULL
    }
    return table;
}

// Simple hash function
unsigned int hash(const char *token) {
    unsigned int hash_value = 0;
    while (*token) {
        hash_value = (hash_value << 5) + *token++; // Shift and add
    }
    return hash_value % TABLE_SIZE; // Return hash value within table size
}

// Insert a token into the hash table
void insert_token(HashTable *table, const char *token) {
    unsigned int index = hash(token); // Get index from hash function
    HashEntry *entry = table->entries[index];

    // Check if token already exists in the chain
    while (entry != NULL) {
        if (strcmp(entry->token, token) == 0) {
            entry->count++; // Increment count if found
            return;
        }
        entry = entry->next; // Move to the next entry in case of collision
    }

    // Token not found, create a new entry
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    strcpy(new_entry->token, token);
    new_entry->count = 1; // Set initial count to 1
    new_entry->next = table->entries[index]; // Insert at the beginning of the chain
    table->entries[index] = new_entry;
}

// Get the count of a token
int get_token_count(HashTable *table, const char *token) {
    unsigned int index = hash(token);
    HashEntry *entry = table->entries[index];

    // Search for the token in the chain
    while (entry != NULL) {
        if (strcmp(entry->token, token) == 0) {
            return entry->count; // Return the count if found
        }
        entry = entry->next;
    }
    return 0; // Return 0 if not found
}

// Free the memory used by the hash table
void free_hash_table(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry *entry = table->entries[i];
        while (entry != NULL) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp); // Free each entry
        }
    }
    free(table->entries); // Free the array of entries
    free(table); // Free the hash table itself
}
