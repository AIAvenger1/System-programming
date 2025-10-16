#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "visualize.h"
#include "codegen.h"
#include "parser.tab.h"

extern FILE* yyin;
extern Node* ast_root;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    printf("Parsing file: %s\n", argv[1]);
    int result = yyparse();
    fclose(yyin);

    if (result == 0 && ast_root != NULL) {
        printf("Parsing completed successfully.\n\n");

        printf("Generating AST visualization...\n");
        visualize_ast(ast_root, "ast.dot");
        system("dot -Tpng ast.dot -o ast.png");
        printf("AST saved to ast.dot and ast.png\n\n");

        generate_code(ast_root);

        free_ast(ast_root);
    } else {
        fprintf(stderr, "Parsing failed.\n");
    }

    return result;
}