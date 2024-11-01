#ifndef PLAGIARISM_CHECK_H
#define PLAGIARISM_CHECK_H

#include "hash_table.h"

// Function to calculate the plagiarism percentage based on token frequency
double calculate_plagiarism_percentage(HashTable *table1, HashTable *table2);


#endif // PLAGIARISM_CHECK_H
