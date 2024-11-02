#include <stdio.h>
#include <stdlib.h>
#include "file_util.h"
#include "tokenizer.h"
#include "hash_table.h"
#include "plagiarism_check.h"
#include "ngram.h" 
#include "helper.h"
#include "ast.h"
#include "parser.h"
#include <time.h>
#include <math.h>
// Main function
//argc-->argument count for input files, it is = 3 as we are taking 2 input files, i.e ./plagiarism_detector test1.c test2.c (3 arguments)
//argv --> here, argv[0] = ./plagiarism_detector, argv[1] = test1.c, argv[2] = test2.c
//time start


// Rest of the code

int main(int argc, char *argv[]) {
    clock_t start_time = clock();
    //argc should be 3, if not then print the usage of the program
    if (argc != 3) {
        printf("Usage: %s <file1.c> <file2.c>\n", argv[0]);
        return 1;
    }

    // Read the contents of the two files
    // read_file: A function (defined in file_util.h) that reads the entire content of the provided file into a string.

    char *content1 = read_file(argv[1]);
    char *content2 = read_file(argv[2]);

    if (content1 == NULL || content2 == NULL) {
        printf("Error reading files.\n");
        return 1;
    }

    // Tokenize the content of both files
    // TokenNode: A linked list structure to store tokens.
    TokenNode *tokens1 = NULL;
    TokenNode *tokens2 = NULL;
    // tokenize_code: Function that takes the file content, extracts meaningful tokens, and stores them in linked lists.
    tokenize_code(content1, &tokens1);
    tokenize_code(content2, &tokens2);

    // Create hash tables for both tokenized contents
    HashTable *table1 = create_hash_table();
    HashTable *table2 = create_hash_table();

    // Insert tokens into the hash tables
    TokenNode *temp = tokens1;
    while (temp != NULL) {
        insert_token(table1, temp->token);  //insert token into hash table
        temp = temp->next;
    }

    temp = tokens2;
    while (temp != NULL) {
        insert_token(table2, temp->token);
        temp = temp->next;
    }

    // Calculate frequency-based plagiarism percentage using hash tables, using Jaccard similarity
    double frequency_percentage = calculate_plagiarism_percentage(table1, table2);
    printf("Frequency-Based Plagiarism Percentage: %.2f%%\n", frequency_percentage);

    // Generate n-grams for both token lists
    int num_tokens1 = count_tokens(tokens1);
    int num_tokens2 = count_tokens(tokens2);
    int num_ngrams1, num_ngrams2;

    // Convert linked list to array and generate n-grams
    char **token_array1 = linked_list_to_array(tokens1);
    char **token_array2 = linked_list_to_array(tokens2);

    char **ngrams1 = generate_ngrams(token_array1, num_tokens1, &num_ngrams1);
    char **ngrams2 = generate_ngrams(token_array2, num_tokens2, &num_ngrams2);

    // Calculate sequence-based similarity using n-grams
    double ngram_percentage = ngram_similarity(ngrams1, num_ngrams1, ngrams2, num_ngrams2);
    printf("n-gram Sequence Similarity Percentage: %.2f%%\n", ngram_percentage);

    // **AST Comparison**
   
    ASTNode *ast1 = parse_source_file(argv[1]);  
ASTNode *ast2 = parse_source_file(argv[2]);  

    if (ast1 == NULL || ast2 == NULL) {
        printf("Error generating ASTs, its NULL.\n");
        return 1;
    }

    double ast_similarity = compare_asts(ast1, ast2);
    printf("AST-Based Similarity Percentage: %.2f%%\n", ast_similarity);

    
    // Combine the results for a final score (weighting all metrics equally)
    double final_percentage = pow(0.2*pow(frequency_percentage,2) + 0.3*pow(ngram_percentage, 2) + 0.5*pow(ast_similarity, 2), 0.5);
    printf("Final Plagiarism Score: %.2f%%\n", final_percentage);

   

    // Free all allocated memory
    free(content1);
    free(content2);
    free_list(tokens1);
    free_list(tokens2);
    free_hash_table(table1);
    free_hash_table(table2);
    free_ngrams(ngrams1, num_ngrams1);
    free_ngrams(ngrams2, num_ngrams2);
    free_token_array(token_array1, num_tokens1);
    free_token_array(token_array2, num_tokens2);
    free_ast(ast1);
    free_ast(ast2);
// Calculate the total runtime
clock_t end_time = clock();
double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
printf("Total Runtime: %.2f seconds\n", total_time);

return 0;

}
