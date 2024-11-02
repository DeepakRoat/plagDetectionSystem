#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char *token;
    char *value;
    struct ASTNode *left;        // First child
    struct ASTNode *right;       // Sibling
    struct ASTNode *lastChild;   // Pointer to the last child for fast insertion avoiding O(n^2) complexity
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
