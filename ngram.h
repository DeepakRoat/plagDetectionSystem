#ifndef NGRAM_H
#define NGRAM_H

// Define the n-gram size (can be changed as needed)
#define N 5

// Function to generate n-grams from an array of tokens
char **generate_ngrams(char **tokens, int num_tokens, int *num_ngrams);

// Function to calculate similarity percentage between two sets of n-grams
double ngram_similarity(char **ngrams1, int num_ngrams1, char **ngrams2, int num_ngrams2);

// Function to free the memory allocated for n-grams
void free_ngrams(char **ngrams, int num_ngrams);

#endif
