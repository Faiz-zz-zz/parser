#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>
#include <stdbool.h>     /* malloc, free, rand */

/*  The main program calls procedures parse, partone, parttwo and bin which are not implemented here.
*/
char *slicer(char *g, start);
int atomic_formula(char *g);
int filtering(char *g, int start);
int existential_formula(char *g);
int univeral_formula(char *g);
int parse(char *g);
int multilple_formulas(char *g);
char *parttwo(char *g);
char *partone(char *g);



int Fsize=50;
int no_edges;
int no_nodes;  /*big enough for our formulas*/

char *slicer(char *g, start){
    char *part=malloc(Fsize);
    int j=0;
    for(int i=start; i<strlen(g); i++)
    {
        part[j++]=g[i];
    }
    return part;
}

int atomic_formula(char *g){
    if (g[1]=='['){
        if (g[4]==']'){
            if((g[3]=='x'||g[3]=='y'||g[3]=='z')&&(g[2]=='x'||g[2]=='y'||g[2]=='z')){
                return 1;
            }
        }
    }
    return 0;
}

int filtering(char *g, int start)
{
    int i=0, flag1=1, flag2=1, flag3=1;
    while(g[i]!='\0')
    {
        if (g[i]!='E'&&g[i]!='A'&&g[i]!='x'&&g[i]!='y'&&g[i]!='z'&&g[i]!='('&&g[i]!=')'&&g[i]!='['&&g[i]!=']'&&g[i]!='X'&&g[i]!='v'&&g[i]!='^'&&g[i]!='>'&&g[i]!='-')
        {
            flag1=0;
        }

        if ((g[i]=='x'||g[i]=='y'||g[i]=='z')&&g[i+1]==')') flag2=0;
        if ((g[i]=='E'||g[i]=='X'||g[i]=='A')&&g[i+1]==')') flag3=0;
        i++;
    }
    // printf("%i  %i  %i\n", flag1, flag2, flag3);
    return flag1&&flag2&&flag3;
}

int existential_formula(char *g){
    if (g[1]=='x'||g[1]=='y'||g[1]=='z'){
        return parse(slicer(g, 2));
    }
}

int universal_formula(char *g){
    if (g[1]=='x'||g[1]=='y'||g[1]=='z'){
        return parse(slicer(g, 2));
    }
}

int parse(char *g)
/* returns 0 for non-formulas, 1 for atoms, 2 for negations, 3 for binary connective fmlas, 4 for existential and 5 for universal formulas.*/
{
    if (filtering(g, 0)>0){ 
        if (g[0]=='X'){
            // printf("I was in this place %i\n", atomic_formula(g));
            return atomic_formula(g);
        }
        else if (g[0]=='E'){
            if (existential_formula(g)>0) return 4;
            else return 0;
        }
        else if (g[0]=='A'){
            if (universal_formula(g)>0) return 5;
            else return 0;
        }
        else if (g[0]=='('){
            if ((parse(partone(g))&&parse(parttwo(g)))>0) return 3;
            else return 0;
        }
        else if (g[0]=='-'){
            if (parse(slicer(g, 1))>0) return 2;
            else return 0;
        }
    }
}
int multilple_formulas(char *g)
{
    int counter=0;
    for (int i=0; i<strlen(g); i++){
        if (g[i]=='(') counter++;
        else if(g[i]==')') counter--;
        
        if ((g[i]=='>'||g[i]=='^'||g[i]=='v')&&counter==1) return i;    
    }
}  

char *parttwo(char *g)
{
    char *part=malloc(Fsize);
    int j=0;
    for(int i=multilple_formulas(g)+1; i<strlen(g)-1; i++)
    {
      part[j++]=g[i];
    }
    return part;
}

char *partone(char *g)
{
    char *part=malloc(Fsize);
    int j=0;
    for(int i=1; i<multilple_formulas(g); i++)
    {
      part[j++]=g[i];
    }
    return part;
}

char bin(char *g)
{
    return g[multilple_formulas(g)];
}

int main()
{
  /*Input a string and check if its a formula*/
  char *name=malloc(Fsize);
  printf("Enter a formula:");
  scanf("%s", name);
  int p=parse(name);
  switch(p)
    {case 0: printf("Not a formula");break;
    case 1: printf("An atomic formula");break;
    case 2: printf("A negated formula");break;
    case 3: printf("A binary connective formula");break;
    case 4: printf("An existential formula");break;
    case 5: printf("A universal formula");break;
    default: printf("Not a formula");break;
    }

  if (p==3)
    {printf("The first part is %s, the binary connective is %c, the second part is %s", partone(name), bin(name), parttwo(name));

  return(1);
}
}