#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char *token;             // Type of the node (e.g., "Function", "Return")
    char *value;             // Original code representation
    struct ASTNode *left;    // Pointer to the left child
    struct ASTNode *right;   // Pointer to the right child
} ASTNode;

// Function prototypes
ASTNode *create_ast_node(const char *token, const char *value);
void add_child(ASTNode *parent, ASTNode *child);
void free_ast(ASTNode *node);
double compare_asts(ASTNode *ast1, ASTNode *ast2); // Added threshold
int count_matching_tokens(ASTNode *ast1, ASTNode *ast2);
int count_total_nodes(ASTNode *node);
void normalize_ast(ASTNode *node); // New function for normalization


#endif // AST_H
