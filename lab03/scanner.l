%{
#include <string.h>
#include "parser.tab.h"

void yyerror(const char* s);
%}

%option yylineno

%%
"/*"([^*]|"*"+[^*/])*"*"+"/"  ; 
"//".* ; 

"int"           { return T_INT; }
"if"            { return T_IF; }
"else"          { return T_ELSE; }
"while"         { return T_WHILE; }
"return"        { return T_RETURN; }
"print"         { return T_PRINT; } 

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_val = strdup(yytext);
    return T_IDENTIFIER;
}

[0-9]+ {
    yylval.int_val = atoi(yytext);
    return T_NUMBER;
}

"==" { return T_EQ; }
"!=" { return T_NEQ; }
"<=" { return T_LE; }
">=" { return T_GE; }
"&&" { return T_AND; }
"||" { return T_OR; }

[ \t\n\r]+ ; 

.    { return yytext[0]; } 
%%

int yywrap(void) {
    return 1;
}