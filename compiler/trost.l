%option noyywrap nodefault yylineno

D  [0-9]
L  [a-zA-Z_]
H  [a-fA-F0-9]
E  [Ee][+-]?{D}+
FS (f|F|l|L)

%{
#include <stdlib.h>
#include "trost.h"
#include "trost.tab.hpp"
%}

%%

"+"                      { return ADD; }
"-"                      { return SUB; }
"*"                      { return MUL; }
"/"                      { return DIV; }
"="                      { return EQ; }
"|"                      { return PIPE; }
","                      { return COMMA; }
"."                      { return DOT; }
";"                      { return SEMI; }
"("                      { return LPAREN; }
")"                      { return RPAREN; }
"["                      { return LBRACK; }
"]"                      { return RBRACK; }
"{"                      { return LBRACE; }
"}"                      { return RBRACE; }
">"                      { return GT; }
"<"                      { return LT; }
"=="                     { return EQ2; }
">="                     { return GE; }
"<="                     { return LE; }
"->"                     { return FUNC; }
"_"                      { return UNDERSCORE; }

"if"                     { return IF; }
"then"                   { return THEN; }
"else"                   { return ELSE; }
"for"                    { return FOR; }
"while"                  { return WHILE; }
"in"                     { return IN; }
"type"                   { return TYPE; }
"int"                    { return INT; }
"float"                  { return FLOAT; }
"string"                 { return STRING; }
"bool"                   { return BOOL; }
"true"                   { yylval.b = true; return BOOL_T; }
"false"                  { yylval.b = false; return BOOL_F; }

"print!"                 { return PRINT; }

{L}({L}|{D})*            { return NAME; }

0[xX]{H}                 { yylval.h = yytext; return HEX_CONSTANT; }
0?D                      { yylval.i = atol(yytext); return INT_CONSTANT; }

{D}+{E}{FS}?           |
{D}*"."{D}+({E})?{FS}? |
{D}+"."{D}*({E})?{FS}?   { yylval.f = atof(yytext); return FLOAT_CONSTANT; }

L?\"(\\.|[^\\"])*\"      { yylval.s = yytext; return STR_CONSTANT; }

"//".*
[ \t\v\f]

\n                       { return EOL; }
.                        { yyerror("Mystery character %c\n", *yytext); }

%%
