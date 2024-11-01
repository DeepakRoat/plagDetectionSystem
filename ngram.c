#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ngram.h"

// Generate n-grams from an array of tokens
char **generate_ngrams(char **tokens, int num_tokens, int *num_ngrams) {
    *num_ngrams = num_tokens - N + 1;
    if (*num_ngrams <= 0) return NULL;  // Handle case with too few tokens

    char **ngrams = (char **)malloc(*num_ngrams * sizeof(char *));
    for (int i = 0; i < *num_ngrams; i++) {
        ngrams[i] = (char *)malloc(256 * sizeof(char));  // Allocate memory for each n-gram
        strcpy(ngrams[i], tokens[i]);  // Copy the first token
        for (int j = 1; j < N; j++) {
            strcat(ngrams[i], " ");
            strcat(ngrams[i], tokens[i + j]);  // Append subsequent tokens
        }
    }
    return ngrams;
}

// Calculate similarity percentage between two sets of n-grams
double ngram_similarity(char **ngrams1, int num_ngrams1, char **ngrams2, int num_ngrams2) {
    int matches = 0;

    // Compare each n-gram from the first set with all n-grams from the second set
    for (int i = 0; i < num_ngrams1; i++) {
        for (int j = 0; j < num_ngrams2; j++) {
            if (strcmp(ngrams1[i], ngrams2[j]) == 0) {
                matches++;
                break;  // Avoid counting the same n-gram multiple times
            }
        }
    }

    // Calculate similarity percentage
    int total = (num_ngrams1 + num_ngrams2) / 2;
    return (matches / (double)total) * 100;
}

// Free the memory allocated for n-grams
void free_ngrams(char **ngrams, int num_ngrams) {
    for (int i = 0; i < num_ngrams; i++) {
        free(ngrams[i]);
    }
    free(ngrams);
}
