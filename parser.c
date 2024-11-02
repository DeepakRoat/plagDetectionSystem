#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ast.h"

// Example function to parse a source file into an AST
ASTNode* parse_source_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Create a root node for the AST
    ASTNode *root = create_ast_node("ROOT", "");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Recognizing function definitions
        if (strstr(line, "int") || strstr(line, "void") || strstr(line, "float") || strstr(line, "char")) {
            ASTNode *func_node = create_ast_node("Function", line);
            add_child(root, func_node);
        }

        // Recognizing variable declarations
        if (strstr(line, "int") || strstr(line, "float") || strstr(line, "char")) {
            if (strchr(line, '=')) {  // Check for initialization
                ASTNode *var_node = create_ast_node("Variable Declaration", line);
                add_child(root, var_node);
            }
        }

        // Recognizing control structures (if-else)
        if (strstr(line, "if(") || strstr(line, "else") || strstr(line, "for(") || strstr(line, "while(")) {
            ASTNode *control_node = create_ast_node("Control Structure", line);
            add_child(root, control_node);
        }

        // Recognizing function calls
        if (strstr(line, "(") && strstr(line, ")")) {
            // Check if it's not a function definition
            if (!strstr(line, "int") && !strstr(line, "void") && !strstr(line, "float") && !strstr(line, "char")) {
                ASTNode *call_node = create_ast_node("Function Call", line);
                add_child(root, call_node);
            }
        }
    }

    fclose(file);
    // Normalize function names in the AST
    normalize_ast(root);
    return root;
}

