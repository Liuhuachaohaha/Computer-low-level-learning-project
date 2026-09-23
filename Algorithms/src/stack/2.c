#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define maxsize 50

typedef struct{
    int* data;
    int top;
}stack;

void initstack(stack *s)
{
    s->top=-1;
    s->data=(int*)malloc(maxsize*sizeof(int));
}

bool stackempty(stack *s)
{
    if(s->top==-1)
    {
        return true;
    }
    else {
        return false;
    }
}

bool push(stack *s,int x)
{
    
}

