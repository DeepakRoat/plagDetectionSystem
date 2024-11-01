#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "tokenizer.h" // Include the tokenizer for token node definition

#define TABLE_SIZE 100 // Define the size of the hash table

// Structure for hash table entry
typedef struct HashEntry {
    char token[100]; // Store the token
    int count;       // Count of occurrences
    struct HashEntry *next; // Pointer to the next entry in case of collision
} HashEntry;

// Structure for the hash table
typedef struct HashTable {
    HashEntry **entries; // Array of hash entries
} HashTable;

// Function prototypes
HashTable* create_hash_table();
unsigned int hash(const char *token);
void insert_token(HashTable *table, const char *token);
int get_token_count(HashTable *table, const char *token);
void free_hash_table(HashTable *table);

#endif  // HASH_TABLE_H
