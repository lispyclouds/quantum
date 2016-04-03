%{
#include "trost.h"
int yylex(void);
%}

%union {
    struct ast *a;
    long i;
    double f;
    char* s;
    char* h;
    bool b;
}

%token ADD SUB MUL DIV
%token EQ PIPE
%token COMMA SEMI DOT UNDERSCORE
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token GT LT EQ2 GE LE
%token FUNC
%token IF THEN ELSE
%token FOR WHILE
%token IN
%token TYPE
%token INT FLOAT STRING BOOL
%token <b> BOOL_T
%token <b> BOOL_F
%token PRINT
%token NAME
%token <h> HEX_CONSTANT
%token <i> INT_CONSTANT
%token <d> FLOAT_CONSTANT
%token <s> STR_CONSTANT
%token EOL

%right EQ
%left ADD SUB
%left MUL DIV
%nonassoc ELSE

%start module

%%

primary_expression
    : NAME
    | HEX_CONSTANT
    | INT_CONSTANT
    | FLOAT_CONSTANT
    | STR_CONSTANT
    | BOOL_T
    | BOOL_F
    ;

postfix_expression
    : primary_expression
    | postfix_expression LBRACK expression RBRACK
    | postfix_expression LPAREN RPAREN
    | postfix_expression LPAREN argument_list RPAREN
    | postfix_expression DOT NAME
    ;

argument_list
    : expression
    | argument_list COMMA expression
    ;

expression
    : expression ADD expression
    | expression SUB expression
    | expression MUL expression
    | expression DIV expression
    | postfix_expression
    | NAME EQ expression
    ;

if_stmt
    : IF expression THEN expression ELSE expression EOL
    ;

for_stmt
    : FOR expression IN expression LBRACE stmt_list RBRACE EOL
    ;

while_stmt
    : WHILE LPAREN expression RPAREN LBRACE stmt_list RBRACE EOL
    ;

stmt
    : if_stmt
    | for_stmt
    | while_stmt
    | expression
    ;

stmt_list
    : stmt
    | stmt_list stmt
    ;

module
    : stmt_list
    ;

%%
