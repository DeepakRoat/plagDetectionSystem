#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Create a new AST node with token and value
ASTNode *create_ast_node(const char *token, const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->token = strdup(token);
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Add a child node to a parent node
void add_child(ASTNode *parent, ASTNode *child) {
    if (parent->left == NULL) {
        parent->left = child;
    } else {
        ASTNode *current = parent->left;
        while (current->right != NULL) {
            current = current->right;
        }
        current->right = child;
    }
}

// Free the memory allocated for the AST
void free_ast(ASTNode *node) {
    if (node) {
        free_ast(node->left);
        free_ast(node->right);
        free(node->token);
        free(node->value);
        free(node);
    }
}

// Normalize the AST by simplifying variable names and function names
void normalize_ast(ASTNode *node) {
   if (node) {
        // Replace variable and function names with generic placeholders
        if (strcmp(node->token, "Variable") == 0 || strcmp(node->token, "Variable Declaration") == 0) {
            free(node->value);
            node->value = strdup("var"); // Generic variable placeholder
        } else if (strcmp(node->token, "Function") == 0 || strcmp(node->token, "Function Call") == 0) {
            free(node->value);
            node->value = strdup("func"); // Generic function placeholder
         } else if (strcmp(node->token, "Control Structure") == 0) { 
            free(node->value);
             node->value = strdup("control"); // Generic control structure placeholder
         } else if (strcmp(node->token, "Literal") == 0) { 
             free(node->value);
             node->value = strdup("const"); // Generic constant placeholder
         }

        // Recursively normalize children
        normalize_ast(node->left);
        normalize_ast(node->right);
    }
}

// Compare two ASTs and return a similarity percentage
double compare_asts(ASTNode *ast1, ASTNode *ast2) {
    if (ast1 == NULL && ast2 == NULL) {
        return 100.0; // Both are empty, hence fully similar
    }
    if (ast1 == NULL || ast2 == NULL) {
        return 0.0; // One is empty, the other is not
    }

    // Normalize ASTs before comparison
    normalize_ast(ast1);
    normalize_ast(ast2);

    // Count matching nodes
    int matching_count = count_matching_tokens(ast1, ast2);

    // Count total unique nodes in both ASTs (union)
    int unique_count_ast1 = count_total_nodes(ast1);
    int unique_count_ast2 = count_total_nodes(ast2);
    
    // Total unique nodes (union)
    int total_unique_count = unique_count_ast1 + unique_count_ast2 - matching_count;

    // Check if total unique count is zero to prevent division by zero
    if (total_unique_count == 0) {
        return 0.0; // Prevent division by zero
    }

    // Calculate Jaccard similarity
    return ((double)matching_count / total_unique_count) * 100.0; // Similarity percentage
}

// Function to count the number of matching nodes between two ASTs
int count_matching_tokens(ASTNode *ast1, ASTNode *ast2) {
    if (ast1 == NULL || ast2 == NULL) {
        return 0;
    }

    int count = 0;

    // Check if both nodes match in type and value
    if (strcmp(ast1->token, ast2->token) == 0) {
        // Check if the values are the same after normalization
        if (strcmp(ast1->value, ast2->value) == 0) {
            count = 1; // Count the match
        }
    }

    // Count matches in left and right children
    count += count_matching_tokens(ast1->left, ast2->left);
    count += count_matching_tokens(ast1->right, ast2->right);

    return count;
}

// Count total nodes in an AST
int count_total_nodes(ASTNode *node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_total_nodes(node->left) + count_total_nodes(node->right);
}
