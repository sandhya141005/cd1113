%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE char*
#include "y.tab.h"
%}
%option noyywrap
%%

"int"|"float"|"char" {return TYPE;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;}
"for"  { return FOR; }
[a-zA-Z_][a-zA-Z0-9_]* {yylval=strdup(yytext); return ID;}
[0-9]+ {yylval=strdup(yytext); return NUM;}
"<="|">="|"=="|"!="|"<"|">" {yylval=strdup(yytext); return RELOP;}
[ \t\r\n]+ ;
. {
    
    return yytext[0];
}
%%