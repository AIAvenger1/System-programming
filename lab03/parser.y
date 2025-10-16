%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char* s);

Node *ast_root = NULL;
%}

%union {
    int int_val;
    char* str_val;
    struct Node* node;
}

%token <int_val> T_NUMBER
%token <str_val> T_IDENTIFIER
%token T_INT T_IF T_ELSE T_WHILE T_RETURN T_PRINT
%token T_EQ T_NEQ T_LE T_GE T_AND T_OR

%type <node> program statements statement declaration assignment_expr if_stmt while_stmt return_stmt print_stmt expr

%left T_OR
%left T_AND
%left T_EQ T_NEQ
%left '<' '>' T_LE T_GE
%left '+' '-'
%left '*' '/'
%right '!' 

%%

program:
    statements { ast_root = create_node(NODE_PROGRAM, $1, NULL, NULL); }
    ;

statements:
    /* empty */       { $$ = NULL; }
    | statement statements { $1->next = $2; $$ = $1; }
    ;

statement:
      declaration ';'    { $$ = $1; }
    | assignment_expr ';' { $$ = $1; }
    | if_stmt            { $$ = $1; }
    | while_stmt         { $$ = $1; }
    | return_stmt ';'    { $$ = $1; }
    | print_stmt ';'     { $$ = $1; }
    | '{' statements '}' { $$ = $2; }
    ;

declaration:
    T_INT T_IDENTIFIER { $$ = create_node(NODE_INT_DECLARATION, create_identifier_node($2), NULL, NULL); free($2); }
    ;

assignment_expr:
    T_IDENTIFIER '=' expr { $$ = create_node(NODE_ASSIGN, create_identifier_node($1), $3, NULL); free($1); }
    ;

if_stmt:
    T_IF '(' expr ')' statement %prec T_IF { $$ = create_node(NODE_IF, $3, $5, NULL); }
    | T_IF '(' expr ')' statement T_ELSE statement { $$ = create_node(NODE_IF_ELSE, $3, $5, $7); }
    ;

while_stmt:
    T_WHILE '(' expr ')' statement { $$ = create_node(NODE_WHILE, $3, $5, NULL); }
    ;

return_stmt:
    T_RETURN expr { $$ = create_node(NODE_RETURN, $2, NULL, NULL); }
    ;

print_stmt:
    T_PRINT '(' expr ')' { $$ = create_node(NODE_PRINT, $3, NULL, NULL); }
    ;

expr:
      T_NUMBER               { $$ = create_constant_node($1); }
    | T_IDENTIFIER           { $$ = create_identifier_node($1); free($1); }
    | expr '+' expr          { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = '+'; }
    | expr '-' expr          { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = '-'; }
    | expr '*' expr          { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = '*'; }
    | expr '/' expr          { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = '/'; }
    | expr '<' expr          { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = '<'; }
    | expr '>' expr          { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = '>'; }
    | expr T_EQ expr         { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = T_EQ; }
    | expr T_NEQ expr        { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = T_NEQ; }
    | expr T_LE expr         { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = T_LE; }
    | expr T_GE expr         { $$ = create_node(NODE_BINARY_OP, $1, $3, NULL); $$->data.int_val = T_GE; }
    | '(' expr ')'           { $$ = $2; }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse Error on line %d near '%s': %s\n", yylineno, yytext, s);
    exit(EXIT_FAILURE);
}