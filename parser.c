#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>
#include <stdbool.h>     /* malloc, free, rand */
/*  The main program calls procedures parse, partone, parttwo and bin which are not implemented here.
*/
int find_paren(char *g, int start);
int paren_single_case(char *g, int start);
int parenvalidity(char *g, int start);
int checkparen(char *g, int start);
int checksqbr(char *g,int start);
int multilple_formulas(char *g, int start);
int formula(char *g,int start);
int parenthesis(char *g, int start);
int existential_or_universal(char *g, int start);
int parsecopy(char *g, int start);
int initial_check(char *g, int start);
int filtering(char *g, int start);

int Fsize=50;  /*big enough for our formulas*/

int find_paren(char *g, int start)/* find position of next closing parenthesis*/
{
	//printf("find %i\n", start);
	int counter=0;
	while(1)
	{
		if (g[start]=='(') counter++;
		else if (g[start]==')') counter--;	
		
		if (counter==0) return start;
		//printf("counter=%i\n", counter);
		start++;	
	}
}

int paren_single_case(char *g, int start)/* check if this the single case where there is no nested parenthesis*/
{										 /* if yes get a return of the position of binary connective*/
	int counter=0, pos;
	//printf("find=%i\t", find_paren(g, start));
	for(int i=start; i<=find_paren(g, start); i++)
	{
		if (g[i]=='v'||g[i]=='^'||g[i]=='>')
		{
			pos=i;
			counter++;
		}
	}
	//printf(" start=%i counter=%i\n", start, counter);
	// printf("counter=%i\n", counter);
	if (counter==0) return 0; /* no binary connctive, hence not valid */
	if (counter>1) return 0; /* more than one binary connectve in side a parenthesis, hence not valid */
	if (counter==1) return pos;
}

int parenvalidity(char *g, int start)  /* only call this function once in the initial call,DO NOT USE FOR RECURSION*/
{
	int counter=0;
	while(g[start]!='\0')
	{
		if (g[start]=='(')
		{
			counter++;
		}
		if (g[start]==')')
		{
			counter--;
			// printf("I was here and counter was %i\n", counter);
			if(counter==0&&g[start+1]=='\0')
			{
				return 1;
			}
		}
		start++;
		//printf("start: %d\n", start);
	}
	// printf("%i\n", counter);
	return 0;
}

int sqbrvalidity(char *g, int start)  /* only call this function once in the initial call,DO NOT USE FOR RECURSION*/
{
	int counter=0;
	while(g[start]!='\0')
	{
		if (g[start]=='[')
		{
			counter++;
		}
		if (g[start]==']')
		{
			counter--;
			if(counter==0&&g[start+1]=='\0')
			{
				return 1;
			}
			else return 0;
		}
		start++;
		//printf("start: %d\n", start);
	}
	// printf("%i\n", counter);
	return 0;
}

int checkparen(char *g, int start) /* check position of next closing sqaure bracket  */
{
	// printf(" start inside the function=%i\n", start);
	int counter=0, i;
	for (i=start; i<=strlen(g); i++)
	{
		// printf("%i\n", i);
		if (g[i]=='(')
		{
			counter++;
		}
		else if (g[i]==')')
		{
			counter--;
			if (counter==1)
			{
				// printf("returned %i\n", i);
				return i+1;
			}
		}
	}
	return -1;
}

int checksqbr(char *g,int start) /*check next closing square bracket*/
{
	for (int i=start; i<strlen(g); i++)
	{
		if (g[i]==']')
		{
			return i;
		}
	}
}

int multilple_formulas(char *g,int start)
{
	int counter=0;
	for(int i=start;i<=find_paren(g, start); i++)
	{
		if(g[i]=='(') counter++;
		if(g[i]==')') counter--;
		if((g[i]=='>'||g[i]=='^'||g[i]=='v')&&counter==1)
		{
			return i;
		}
	}
}    

int formula(char *g,int start) /* basic case of checking if it's a formula */
{
	int end=checksqbr(g, start);
    if (g[start]=='X')
    {
        if (g[start+1]=='[' && g[end]==']') /* && g[end + 1] == '\0' add this back later in to check the end case for each input */
        {
            int flag=1;
            for (int i=start+2; i<end; i++)
            {
                if (g[i]=='x' || g[i]=='y' || g[i]=='z')
                {
                    flag=flag;
                }
                else
                { 
                    flag=0;
                }
            }
            if (flag)
            {
                return 1;
            }
            else
            {
                // printf("debug");
                return 0;
            }
        }
        else 
        {
            return 0;
        }
    }
}

int parenthesis(char *g, int start) /* Parenthesis base function, can be called recursively*/
{
	if (paren_single_case(g, start)>0)
	{
		int flagr=0, flagl=0, pos=paren_single_case(g, start);
		// printf("POS=%i start=%i\n", pos, start);
		if (g[start+1]=='E'||g[start+1]=='A') /* right end of the first case parenthesis, namely, (_binary_) */
		{
			flagr=existential_or_universal(g, start+1);
		}
		else if(g[start+1]=='X')
		{
			flagr=formula(g, start+1);
		}
		else return 0;

		if (g[pos+1]=='E'||g[pos+1]=='A') /* left case */
		{
			flagl=existential_or_universal(g, pos+1);
		}
		else if(g[pos+1]=='X')
		{
			flagl=formula(g, pos+1);
		} 
		else return 0;

		// printf("flagl=%i  flagr=%i\n", flagl, flagr);

		if (flagl>0 && flagr>0)
		{
			return 1;
		}
	}
	else if (multilple_formulas(g, start)>0)
	{
		int flagr=0, flagl=0;
		//printf("start=%i  mult_form=%i\n", start, multilple_formulas(g,start));
		flagr=parsecopy(g, start+1);                     /* flagr is giving 0 as output */
		flagl=parsecopy(g, multilple_formulas(g, start));/* the bug is in find_paren and nect_paren functions*/
		printf("flagr=%i, flagl=%i, start=%i\n", flagr, flagl, start);
		if (flagr>0 && flagl>0) return 1;
		else return 0;
	}
	else return 0;
}

int existential_or_universal(char *g, int start) /* function checking if it's existential or universal */
{
	if (g[start]=='E'|| g[start]=='A')
	{
		if (g[start+1]=='x'||g[start+1]=='y'||g[start+1]=='z')
		{
			if(g[start+2]=='X')
			{
				if (formula(g, (start+2)))
				{
					if(g[start]=='E')
					{
						return 4;
					}
					if(g[start]=='A')
					{
						return 5;
					}
				}
			}
			else if(g[start+2]=='A'||g[start+2]=='E')
			{
				if (existential_or_universal(g, start+2)==5)  /*inital call will differentiate, it doesn't matter what value*/
				{											  /*  as long as the value is more than 0*/
					return 4;
				}
				else if(existential_or_universal(g, start+2)==4)
				{
					return 5;
				}
				else return 0;
			}
			if(g[start+2]=='(')
			{
				// printf("I was inside 271\n");
				return parenthesis(g, start+2);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}

int parsecopy(char *g, int start)
/* returns 0 for non-formulas, 1 for atoms, 2 for negations, 3 for binary connective fmlas, 4 for existential and 5 for universal formulas.*/
{
    if (g[start]=='X')
    {
        return formula(g, start);
    }

    else if (g[start]=='E'||g[start]=='A')
    {
    	if (g[start]=='E')
    	{
    		if (existential_or_universal(g, start)!=0)
    		{
    			return 4;
    		}
    		else return 0;
    	}
    	if (g[start]=='A')
    	{
    		if (existential_or_universal(g, 0)!=0)
    		{
    			return 5;
    		}
    		else return 0;
    	}
	}
	else if (g[start]=='(')
	{
		//printf("I am here 302 %i\n", start);
		if (parenthesis(g, start)>0) return 3;
		else return 0;
	}
	else if(g[start]=='-')
	{
		return parsecopy(g, start+1);
	}
}

int initial_check(char *g, int start)
{
	int i=0;
	while(1)
	{
		if(g[i]=='(')
		{
			return parenvalidity(g, start);
		}
		else if (g[i]=='[')
		{
			return sqbrvalidity(g, start);
		}
		i++;
	}
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

int parse(char *g)
/* returns 0 for non-formulas, 1 for atoms, 2 for negations, 3 for binary connective fmlas, 4 for existential and 5 for universal formulas.*/
{
    if (filtering (g, 0)>0)
    {	
    	if (g[0]=='X'&&initial_check(g, 0)>0)
    	{
        	return formula(g, 0);
    	}

    	else if ((g[0]=='E'||g[0]=='A')&&initial_check(g, 0)>0)
    	{
    		// printf("I was inside\n");
    		if (g[0]=='E')
    		{
    			if (existential_or_universal(g, 0)!=0)
    			{
    				return 4;
    			}
    			else return 0;
    		}
    		if (g[0]=='A')
    		{
    			if (existential_or_universal(g, 0)!=0)
    			{
    				return 5;
    			}
    			else return 0;
    		}
		}
		else if (g[0]=='('&&initial_check(g, 0)>0)
		{
			printf("I was here\n");
			if (parenvalidity(g, 0)==0) return 0;
			else
			{
				if (parenthesis(g, 0)>0) return 3;
				else return 0;
			}
		}
		else if(g[0]=='-'&&initial_check(g, 0))
		{
			if (parsecopy(g, 1)>0)
			{
				return 2;
			}
		}
	}
}

char *parttwo(char *g)
// Given a formula (A*B) this should return A
{
	char *part=malloc(Fsize);
	int j=0;
	for(int i=multilple_formulas(g, 0)+1; i<strlen(g); i++)
	{
		part[j++]=g[i];
	}
	return part;
}

char *partone(char *g)
/*
Given a formula (A*B) this should return B
 */
{
	char *part=malloc(Fsize);
	int j=0;
	for(int i=1; i<multilple_formulas(g, 0); i++)
	{
		part[j++]=g[i];
	}
	return part;
}

char bin(char *g)
/*
Given a formula (A*B) this should return the character *
 */
{
	return g[multilple_formulas(g, 0)];
}
 

int main()
{
  /*Input a string and check if its a formula*/
  char *name=malloc(Fsize);
  printf("Enter a formula:");
  scanf("%s", name);
  int p = parse(name);
  
  switch(p) {
  	case 0: printf("Not a formula");break;
    case 1: printf("An atomic formula");break;
    case 2: printf("A negated formula");break;
    case 3: printf("A binary connective formula");break;
    case 4: printf("An existential formula");break;
    case 5: printf("A universal formula");break;
    default: printf("Not a formula");break;
   }

  if (p==3)
    {
    	printf("The first part is %s, the binary connective is %c, the second part is %s", partone(name), bin(name), parttwo(name));
    }
  return 0;
}
