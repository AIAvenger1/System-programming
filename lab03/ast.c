#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Node* create_node(NodeType type, Node* op1, Node* op2, Node* op3) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    new_node->type = type;
    new_node->op1 = op1;
    new_node->op2 = op2;
    new_node->op3 = op3;
    new_node->next = NULL;
    return new_node;
}

Node* create_constant_node(int value) {
    Node* new_node = create_node(NODE_CONSTANT, NULL, NULL, NULL);
    new_node->data.int_val = value;
    return new_node;
}

Node* create_identifier_node(char* name) {
    Node* new_node = create_node(NODE_IDENTIFIER, NULL, NULL, NULL);
    new_node->data.str_val = strdup(name);
    return new_node;
}

void free_ast(Node* node) {
    if (!node) return;
    free_ast(node->op1);
    free_ast(node->op2);
    free_ast(node->op3);
    free_ast(node->next);
    if (node->type == NODE_IDENTIFIER) {
        free(node->data.str_val);
    }
    free(node);
}