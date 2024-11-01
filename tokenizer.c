//extract tokens from the input code files
//storing these tokens in a linked list for further comparison
//it only handels meneningful tokens i.e ignores comments, white spaces, etc
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"  // Include the header file

 // building a linked list of tokens

// Create a new node to store a token
//const char *token is a string representing the token to be stored in the new node.
TokenNode* create_node(const char *token) {
    TokenNode *new_node = (TokenNode*)malloc(sizeof(TokenNode));
    strcpy(new_node->token, token);   //copy the entire string of token parameter to the token field of the new node
    new_node->next = NULL;     //Initializing the Next Pointer to NULL
    return new_node;     //return type: a pointer to a TokenNode
}
// Add a token to the linked list
void add_token(TokenNode **head, const char *token) {
    TokenNode *new_node = create_node(token);

    if (*head == NULL) {
        *head = new_node;
    } else {
        TokenNode *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}
// Print all tokens in the linked list (for debugging)
void print_tokens(TokenNode *head) {
    TokenNode *temp = head;
    while (temp != NULL) {
        printf("%s\n", temp->token);
        temp = temp->next;
    }
}

// Free the memory used by the linked list
void free_list(TokenNode *head) {
    TokenNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
// Convert linked list to array of strings
char **linked_list_to_array(TokenNode *head) {
    int num_tokens = 0;
    TokenNode *temp = head;

    // Count the number of tokens
    while (temp != NULL) {
        num_tokens++;
        temp = temp->next;
    }

    // Allocate memory for the array of strings
    char **array = (char **)malloc(num_tokens * sizeof(char *));
    temp = head;
    for (int i = 0; i < num_tokens; i++) {
        array[i] = strdup(temp->token);  // Copy token to array
        temp = temp->next;
    }
    return array;
}

// Free the memory of the token array
void free_token_array(char **array, int num_tokens) {
    for (int i = 0; i < num_tokens; i++) {
        free(array[i]);  // Free each string
    }
    free(array);  // Free the array itself
}

#include <string.h>
#include <stdio.h>

const char *common_tokens[] = {
    "int", "return", "void", "printf", "scanf", "for", "if", "else", "while",
    "break", "continue", "char", "double", "float", "include", "define", "static",
    "const", "long", "short", "sizeof", "main", "true", "false", "null", 
    "+", "-", "*", "/", "=", "==", ">", "<", "&&", "||", "!", "{", "}", "(", ")", ";", ",", "//"
};
// Normalize tokens (convert to lowercase)
void normalize_token(char *token) {
    for (int i = 0; token[i]; i++) {
        token[i] = tolower((unsigned char)token[i]);
    }
}

#define COMMON_TOKENS_COUNT (sizeof(common_tokens) / sizeof(common_tokens[0]))

int is_common_token(const char *token) {
    for (int i = 0; i < COMMON_TOKENS_COUNT; i++) {
        if (strcmp(token, common_tokens[i]) == 0) {
            return 1; // Token is common
        }
    }
    return 0; // Token is not common
}

// Count the number of tokens in the linked list
int count_tokens(TokenNode *head) {
    int count = 0;
    TokenNode *temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

void tokenize_code(const char *content, TokenNode **head) {
    // Initial buffer size
    int buffer_size = 100;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        perror("Failed to allocate buffer");
        return;
    }
    
    int j = 0; // Index for the buffer
    int in_comment = 0; // Flag to track if we are inside a comment

    for (int i = 0; content[i] != '\0'; i++) {
        // Skip single-line comments
        if (content[i] == '/' && content[i + 1] == '/') {
            while (content[i] != '\n' && content[i] != '\0') {
                i++;
            }
            continue; // Skip this line
        }

        // Handle multi-line comments
        if (content[i] == '/' && content[i + 1] == '*') {
            in_comment = 1; // Start of multi-line comment
            i += 2; // Skip past "/*"
            while (in_comment && content[i] != '\0') {
                if (content[i] == '*' && content[i + 1] == '/') {
                    in_comment = 0; // End of multi-line comment
                    i += 2; // Skip past "*/"
                } else {
                    i++;
                }
            }
            continue; // Skip processing this comment
        }

        // Process tokens only if we are not in a comment
        if (!in_comment) {
            // Check if the character is part of a token
            if (isalnum(content[i]) || content[i] == '_') {
                // Check if we need to expand the buffer
                if (j >= buffer_size - 1) { // -1 for null terminator
                    buffer_size *= 2; // Double the buffer size
                    char *new_buffer = realloc(buffer, buffer_size); // Reallocate buffer
                    if (!new_buffer) {
                        perror("Reallocation failed");
                        free(buffer); // Free the old buffer
                        return;
                    }
                    buffer = new_buffer; // Update buffer pointer
                }
                buffer[j++] = content[i]; // Add character to buffer
            } else {
                // If we have collected a token, process it
                if (j > 0) {
                    buffer[j] = '\0'; // Null-terminate the string
                    normalize_token(buffer);
                    if (!is_common_token(buffer)) {
                        add_token(head, buffer);
                    }
                    j = 0; // Reset index for the next token
                }
                // Handle operators and punctuation
                if (!isspace(content[i])) {
                    char op[2] = {content[i], '\0'};
                    normalize_token(op); // Normalize single-character tokens
                    if (!is_common_token(op)) {
                        add_token(head, op);
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
            add_token(head, buffer);
        }
    }

    free(buffer); // Free the dynamically allocated buffer
}