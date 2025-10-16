#include <stdio.h>
#include "codegen.h"
#include "parser.tab.h"

static int label_counter = 0;

static void codegen_recursive(Node* node) {
    if (!node) return;
    int label1, label2;

    switch (node->type) {
        case NODE_PROGRAM:
            codegen_recursive(node->op1);
            break;
        case NODE_INT_DECLARATION:
            break;
        case NODE_ASSIGN:
            codegen_recursive(node->op2);
            printf("STORE %s\n", node->op1->data.str_val);
            break;
        case NODE_IF:
            label1 = label_counter++;
            codegen_recursive(node->op1);
            printf("JUMP_IF_FALSE L%d\n", label1);
            codegen_recursive(node->op2);
            printf("L%d:\n", label1);
            break;
        case NODE_IF_ELSE:
            label1 = label_counter++;
            label2 = label_counter++;
            codegen_recursive(node->op1);
            printf("JUMP_IF_FALSE L%d\n", label1);
            codegen_recursive(node->op2);
            printf("JUMP L%d\n", label2);
            printf("L%d:\n", label1);
            codegen_recursive(node->op3);
            printf("L%d:\n", label2);
            break;
        case NODE_WHILE:
            label1 = label_counter++;
            label2 = label_counter++;
            printf("L%d:\n", label1);
            codegen_recursive(node->op1);
            printf("JUMP_IF_FALSE L%d\n", label2);
            codegen_recursive(node->op2);
            printf("JUMP L%d\n", label1);
            printf("L%d:\n", label2);
            break;
        case NODE_PRINT:
            codegen_recursive(node->op1);
            printf("PRINT\n");
            break;
        case NODE_CONSTANT:
            printf("PUSH %d\n", node->data.int_val);
            break;
        case NODE_IDENTIFIER:
            printf("LOAD %s\n", node->data.str_val);
            break;
        case NODE_BINARY_OP:
            codegen_recursive(node->op1);
            codegen_recursive(node->op2);
            switch (node->data.int_val) {
                case '+': printf("ADD\n"); break;
                case '-': printf("SUB\n"); break;
                case '*': printf("MUL\n"); break;
                case '/': printf("DIV\n"); break;
                case '<': printf("LT\n"); break;
                case '>': printf("GT\n"); break;
                case T_EQ: printf("EQ\n"); break;
                case T_NEQ: printf("NEQ\n"); break;
                case T_LE: printf("LE\n"); break;
                case T_GE: printf("GE\n"); break;
            }
            break;
        default: break;
    }

    if (node->next) {
        codegen_recursive(node->next);
    }
}

void generate_code(Node* root) {
    printf("--- Generated Stack Machine Code ---\n");
    codegen_recursive(root);
    printf("HALT\n");
    printf("------------------------------------\n");
}