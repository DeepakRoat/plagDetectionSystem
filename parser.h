#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

// Function to parse a C source file and generate an AST
ASTNode* parse_source_file(const char *filename);

#endif // PARSER_H