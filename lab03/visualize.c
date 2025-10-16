#include <stdio.h>
#include "visualize.h"
#include "parser.tab.h"

static int node_counter = 0;

static const char* get_node_label(Node* node) {
    static char buffer[128];
    switch (node->type) {
        case NODE_PROGRAM: return "Program";
        case NODE_STATEMENTS: return "Statements";
        case NODE_INT_DECLARATION: return "int decl";
        case NODE_ASSIGN: return "=";
        case NODE_IF: return "if";
        case NODE_IF_ELSE: return "if-else";
        case NODE_WHILE: return "while";
        case NODE_RETURN: return "return";
        case NODE_PRINT: return "print";
        case NODE_BINARY_OP:
            switch(node->data.int_val) {
                case T_EQ: sprintf(buffer, "op: =="); return buffer;
                case T_NEQ: sprintf(buffer, "op: !="); return buffer;
                case T_LE: sprintf(buffer, "op: <="); return buffer;
                case T_GE: sprintf(buffer, "op: >="); return buffer;
                default: sprintf(buffer, "op: %c", node->data.int_val); return buffer;
            }
        case NODE_CONSTANT:
            sprintf(buffer, "const: %d", node->data.int_val);
            return buffer;
        case NODE_IDENTIFIER:
            sprintf(buffer, "id: %s", node->data.str_val);
            return buffer;
        default: return "Unknown";
    }
}

static void ast_to_dot_recursive(Node* node, FILE* f) {
    if (!node) return;
    int current_id = node_counter++;
    fprintf(f, "  node%d [label=\"%s\"];\n", current_id, get_node_label(node));

    if (node->op1) {
        fprintf(f, "  node%d -> node%d;\n", current_id, node_counter);
        ast_to_dot_recursive(node->op1, f);
    }
    if (node->op2) {
        fprintf(f, "  node%d -> node%d;\n", current_id, node_counter);
        ast_to_dot_recursive(node->op2, f);
    }
    if (node->op3) {
        fprintf(f, "  node%d -> node%d;\n", current_id, node_counter);
        ast_to_dot_recursive(node->op3, f);
    }
    if (node->next) {
        fprintf(f, "  node%d -> node%d [style=dotted, constraint=false];\n", current_id, node_counter);
        ast_to_dot_recursive(node->next, f);
    }
}

void visualize_ast(Node* root, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Could not open file for visualization");
        return;
    }
    fprintf(f, "digraph AST {\n");
    fprintf(f, "  node [shape=box];\n");
    node_counter = 0;
    ast_to_dot_recursive(root, f);
    fprintf(f, "}\n");
    fclose(f);
}