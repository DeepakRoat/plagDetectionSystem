#ifndef TOKENIZER_H  // Include guard to prevent multiple inclusion
#define TOKENIZER_H

// Define the structure for the linked list node
typedef struct TokenNode {
    char token[100];            // Store the token
    struct TokenNode *next;     // Pointer to the next token
} TokenNode;

// Function prototypes
TokenNode* create_node(const char *token);
void add_token(TokenNode **head, const char *token);
void print_tokens(TokenNode *head);
void free_list(TokenNode *head);
void tokenize_code(const char *content, TokenNode **head);
char **linked_list_to_array(TokenNode *head);  // New function
void free_token_array(char **array, int num_tokens);  // New function
int count_tokens(TokenNode *head);  
#endif  // End of include guard
