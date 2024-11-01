#include <stdio.h>
#include "plagiarism_check.h"
#include "hash_table.h"
#include "helper.h" // Include the helper header


// Function to calculate the plagiarism percentage based on token frequency
double calculate_plagiarism_percentage(HashTable *table1, HashTable *table2) {
    int intersection_size = 0;
    int union_size = 0;
    int total_tokens = 0; // Initialize total tokens variable

    // Iterate through all keys in the first hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry *entry = table1->entries[i];
        while (entry != NULL) {
            int count1 = entry->count;
            int count2 = get_token_count(table2, entry->token);

            // Increment intersection and union sizes based on token counts
            intersection_size += min(count1, count2);
            union_size += count1;
            total_tokens += count1; // Count tokens for total
            entry = entry->next;
        }
    }

    // Include remaining tokens from the second hash table in the union
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry *entry = table2->entries[i];
        while (entry != NULL) {
            if (get_token_count(table1, entry->token) == 0) {
                union_size += entry->count;
                total_tokens += entry->count; // Count tokens for total
            }
            entry = entry->next;
        }
    }

    // Calculate and return the plagiarism percentage
    return calculate_jaccard_similarity(intersection_size, union_size, total_tokens); // Call with three parameters
}
