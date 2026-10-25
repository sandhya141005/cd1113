#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ops[] = {"+", "-", "*", "/", "<<"};
char *ins[] = {"ADD", "SUB", "MUL", "DIV", "SHL"};
char *rel[] = {"<", ">", "<=", ">=", "==", "!="}; /* relation -> jump when it is FALSE  */
char *jmp[] = {"JGE", "JLE", "JGT", "JLT", "JNE", "JEQ"};
int idx(char *x[], int k, char *op)
{
    for (int i = 0; i < k; i++)
    {
        if (!strcmp(x[i], op))
            return i;
    }
    return -1;
}

int main()
{
    char line[100], next[100], r[20], a[20], op[20], b[20], L[20];
    int have = (fgets(line, sizeof line, stdin) != NULL);
    while (have)
    {
        have = (fgets(next, sizeof next, stdin) != NULL);
        if(sscanf(line,"%s = %s %s %s",r,a,op,b)==4){
            int k=idx(rel,6,op);
            if(k>=0 && have && sscanf(next,"ifFalse %s goto %s",r,L)==2){
                printf("\tMOV R0, %s\n\tCMP R0, %s\n\t%s %s\n",a,b,jmp[k],L);
                have=(fgets(next,sizeof next, stdin)!=NULL);
            }
            else{
                printf("\tMOV R0, %s\n\t%s R0, %s\n\tMOV %s, R0\n",a,ins[idx(ops,5,op)],b,r);
            }
        }
        else if(sscanf(line,"%s = %s", r, a)==2){
            printf("\tMOV R0, %s\n\tMOV %s, R0\n",a,r);
        }
        else if(sscanf(line,"ifFalse %s goto %s",a,L)==2){
            printf("\tMOV R0, %s\n\tCMP R0, 0\n\tJEQ %s\n",a,L);
        }
        else if(sscanf(line,"goto %s",L)==1){
            printf("\tJMP %s\n", L);
        }
        else
            printf("%s", line);                                    /* a label such as  L1:  */

        strcpy(line, next);
    }
    return 0;
}