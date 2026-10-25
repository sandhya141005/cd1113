#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <ctype.h> 

typedef struct{
    int kind; //0-label/goto,1-assignment,2-binary,3-iffalse goto
    char r[20],a[20],op[4],b[20];
    char raw[40];
} T; 
int n=0; 
T c[300];
char kv[200][20],kc[200][20]; 
int nk=0; 

int isnum(char *s){
    return isdigit(s[0])||s[0]=='-' && isdigit(s[1]);
}
int istemp(char *s){
    return s[0]=='t' && isdigit(s[1]);
}
char *known(char *x){
    for(int i=0;i<nk;i++){
        if(!strcmp(kv[i],x)){
            return kc[i];
        }
    }
    return NULL;
}
void forget(char *x){
    for(int i=0;i<nk;i++){
        if(!strcmp(kv[i],x)||!strcmp(kc[i],x)){
            nk--;
            strcpy(kv[i],kv[nk]);
            strcpy(kc[i],kc[nk]);
            i--;
        }
    }
}
void learn(char *x,char *v){
    forget(x);
    strcpy(kv[nk],x);
    strcpy(kc[nk],v);       
    nk++;
}
int calc(char *op,int x,int y){
    if (!strcmp(op, "+"))
        return x + y;
    if (!strcmp(op, "-"))
        return x - y;
    if (!strcmp(op, "*"))
        return x * y;
    if (!strcmp(op, "/"))
        return y ? x / y : 0;
    if (!strcmp(op, "<"))
        return x < y;
    if (!strcmp(op, ">"))
        return x > y;
    if (!strcmp(op, "<="))
        return x <= y;
    if (!strcmp(op, ">="))
        return x >= y;
    if (!strcmp(op, "=="))
        return x == y;
    return x != y;
}
void to_copy(T *t,char *v){
    strcpy(t->a,v);
    t->kind=1;
}
void simplify(T *t){
    char tmp[20];
    int ca=isnum(t->a),cb=isnum(t->b),p;
    //binary with 2 numbers - constant folding
    if(ca  && cb){
        sprintf(tmp,"%d",calc(t->op,atoi(t->a),atoi(t->b)));
        to_copy(t,tmp);
        return;
    }
    //keep constant on rhs of the equation
    if((t->op[0]=='+'|| t->op[0]=='*') && ca){
        strcpy(tmp,t->a);
        strcpy(t->a,t->b);
        strcpy(t->b,tmp);
        cb=1;
    }
    if(!cb) return;
    if ((!strcmp(t->op, "+") || !strcmp(t->op, "-")) && !strcmp(t->b, "0"))
        to_copy(t, t->a); /* 2. algebraic: x+0  x-0 -> x */
    else if ((!strcmp(t->op, "*") || !strcmp(t->op, "/")) && !strcmp(t->b, "1"))
        to_copy(t, t->a); /*    x*1  x/1 -> x */
    else if (!strcmp(t->op, "*") && !strcmp(t->b, "0"))
        to_copy(t, "0"); /*    x*0 -> 0 */
    else if(!strcmp(t->op,"*")){
        for(p=1;p<15;p++){
            if(atoi(t->b)==(1<<p)){
                strcpy(t->op,"<<");
                sprintf(t->b,"%d",p);
                break;
            }
        }
    }
}
int main(){
    char line[100];
    while (fgets(line,sizeof(line),stdin)){
        T *t=&c[n++];
        if(sscanf(line,"%s=%s %s %s",t->r,t->a,t->op,t->b)==4){
           t->kind=2;
        }
         else if (sscanf(line, "%s = %s", t->r, t->a) == 2)
            t->kind = 1;
        else if (sscanf(line, "ifFalse %s goto %s", t->a, t->b) == 2)
            t->kind = 3;
        else{
            sscanf(line,"%[^\n]",t->raw);
            t->kind=0;
        }
    }

    for(int i=0;i<n;i++){
        T *t=&c[i]; 
        if(t->kind==0 && strchr(t->raw,':')){
            nk=0;
            continue;
        }
        if(t->kind==0) continue;

        char *v;
        if((v=known(t->a))!=NULL){
            strcpy(t->a,v);
        }

        if(t->kind==3) continue; 

        if(t->kind==2){
            if ((v = known(t->b)) != NULL)
                strcpy(t->b, v);
            simplify(t);
        }
        if(t->kind==1 && (isnum(t->a)||istemp(t->r))){
            learn(t->r,t->a);
        }
        else{
            forget(t->r);
        }
    }

    for(int i=0;i<n;i++){
        T *t=&c[i]; 
        if(t->kind==0|| t->kind==3|| !istemp(t->r)) continue;
        int used=0;
        for(int j=0;j<n;j++){
            if(j==i||c[j].kind==0) continue;
            if(!strcmp(c[j].a,t->r)||(c[j].kind != 1 && !strcmp(c[j].b, t->r))){
                used=1;
            }
        }
        if (!used)
            t->kind = -1;
    }
    for (int i = 0; i < n; i++)
    { /* ---- print ---- */
        T *t = &c[i];
        if (t->kind == 0)
            printf("%s\n", t->raw);
        if (t->kind == 1)
            printf("%s = %s\n", t->r, t->a);
        if (t->kind == 2)
            printf("%s = %s %s %s\n", t->r, t->a, t->op, t->b);
        if (t->kind == 3)
            printf("ifFalse %s goto %s\n", t->a, t->b);
    }
    return 0;
}
