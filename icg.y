%{
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE char* 
int yylex(void);
void yyerror(const char *s){
    fprintf(stderr,"Syntax err:%s\n",s);
}
int t=0,l=0;
char *newtemp(){
    char *s=malloc(8);
    sprintf(s,"t%d",++t);
    return s;
}
char *newlabel(){
    char *s=malloc(8);
    sprintf(s,"L%d",++l);
    return s;
}
char *gen(char *a,char *op,char *b){
    char *s=newtemp();
    printf("%s = %s %s %s\n",s,a,op,b);
    return s;
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
ifhead: IF '(' expr ')' {$$=newlabel(); printf("ifFalse %s goto %s\n",$3,$$);};

stmt: TYPE list ';'
    | TYPE ID '=' expr ';' {printf("%s=%s\n",$2,$4);}
    | assign ';' 
    | ifhead stmt %prec IFX {printf("%s:\n",$1);}
    |ifhead stmt ELSE  {$$=newlabel(); printf("goto %s\n%s:\n",$$,$1);}
     stmt              {printf("%s:\n",$4);}
    |WHILE             {$$=newlabel(); printf("%s:\n",$$);}
    '('expr')'         {$$=newlabel(); printf("ifFalse %s goto %s\n",$4,$$);}
    stmt               {printf("goto %s\n%s:\n",$2,$6);}
    |FOR '('assign ';' {$$=newlabel();printf("%s:\n",$$);}
    expr ';'           {$$=newlabel();printf("ifFalse %s goto %se\n goto %sb\n %ss: \n",$6,$$,$$,$$);} 
    assign ')'         {printf("goto %s\n%sb:\n",$5,$8);}
    stmt               { printf("goto %ss\n%se:\n", $8, $8); }
    |'{'prog'}'
    ;
list:ID|list','ID;
assign : ID '=' expr   { printf("%s = %s\n", $1, $3); } ;
expr : expr RELOP expr  { $$ = gen($1, $2,  $3); }
     | expr '+' expr    { $$ = gen($1, "+", $3); }
     | expr '-' expr    { $$ = gen($1, "-", $3); }
     | expr '*' expr    { $$ = gen($1, "*", $3); }
     | expr '/' expr    { $$ = gen($1, "/", $3); }
     | '(' expr ')'     { $$ = $2; }
     | ID               { $$ = $1; }
     | NUM              { $$ = $1; }
     ;
%%

int main() { yyparse(); return 0; }