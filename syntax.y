%{
#include <stdio.h>
#define YYSTYPE char*
#include "y.tab.h"
int yylex(void);
void yyerror(const char *s){
    printf("Syntax error: %s\n",s);
}
%}
%token TYPE IF ELSE WHILE FOR ID NUM RELOP
%nonassoc IFX
%nonassoc ELSE 
%left RELOP 
%left '+' '-' 
%left '*' '/'
%% 

prog: prog stmt|;
stmt: TYPE list';'
    | TYPE ID '=' expr ';'
    | ID '=' expr ';'
    |IF '(' expr ')' stmt %prec IFX 
    |IF '(' expr ')' stmt ELSE stmt 
    |WHILE '(' expr ')' stmt 
    |FOR '('assign';'expr';'assign')' stmt 
    |'{' prog '}'
    ;
list: ID|list ',' ID;
assign:ID'='expr;
expr: expr RELOP expr 
    | expr '+' expr 
    |expr '-' expr 
    |expr '*' expr 
    |expr '/' expr 
    |'(' expr ')'
    |ID 
    |NUM 
    ;
%% 

int main(){
    if (yyparse()==0) printf("Syntax valid\n");
    return 0;
}