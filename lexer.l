%{
#include <stdio.h>
%}
%option noyywrap
%%

"int"|"float"|"char"|"if"|"else"|"while"|"for" {printf("%-10s Keyword\n",yytext);}
[a-zA-Z_][a-zA-Z0-9_]* {printf("%-10s IDENTIFIER\n",yytext);}
[0-9]+ {printf("%-10s NUMBER\n",yytext);}
"<="|">="|"=="|"!="|"<"|">" {printf("%-10s REL OP\n",yytext);}
"+"|"-"|"*"|"/"|"=" {printf("%-10s OPERATOR\n",yytext);}
[{}();,] {printf("%-10s SEPARATOR\n",yytext);}
[ \t\n]+ {}
. {printf("%-10s UNKNOWN\n",yytext);}
%%

int main(){
    yylex();
    return 0;
}