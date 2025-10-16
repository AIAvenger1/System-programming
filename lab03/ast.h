#pragma once

typedef enum {
    NODE_PROGRAM,
    NODE_STATEMENTS,
    NODE_INT_DECLARATION,
    NODE_ASSIGN,
    NODE_IF,
    NODE_IF_ELSE,
    NODE_WHILE,
    NODE_RETURN,
    NODE_PRINT,
    NODE_BINARY_OP,
    NODE_CONSTANT,
    NODE_IDENTIFIER
} NodeType;

typedef struct Node {
    NodeType type;
    struct Node *op1, *op2, *op3;
    struct Node *next;
    union {
        int int_val;
        char* str_val;
    } data;
} Node;

Node* create_node(NodeType type, Node* op1, Node* op2, Node* op3);
Node* create_constant_node(int value);
Node* create_identifier_node(char* name);

void free_ast(Node* node);