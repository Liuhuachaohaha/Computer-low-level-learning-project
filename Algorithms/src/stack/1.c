#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int* data;
    int top;
    int capacity;
}stack;

void initstack(stack* s,int capacity)
{
    s->data=(int*)malloc(capacity*sizeof(int));
    s->top=-1;
    s->capacity=capacity;
}

void printstack(stack* s)
{
    printf("stack: ");
    for(int i=0;i<s->top;i++)
    {
        printf("%d ",s->data[i]);
    }
    printf(" top=%d",s->top);
    printf(" capacity=%d\n",s->capacity);
}

void push(stack* s,int val)
{
    
    s->top++;
    if(s->top>=s->capacity)
    {
        printf("stack overflow\n");
        s->top--;
        return;
    }
    s->data[s->top]=val;
}

int pop(stack* s)
{
    if(s->top==-1)
    {
        printf("stack underflow\n");
        return -1;
    }
    int val=s->data[s->top];
    s->top--;
    return val;
}
void peek(stack* s)
{
    if(s->top==-1)
    {
        printf("stack underflow\n");
        return;
    }
    printf("peek %d\n",s->data[s->top]);
}

void destory(stack* s)
{
    free(s->data);
}
int main()
{
    stack s;
    initstack(&s,10);
    printstack(&s);
    push(&s,1);
    printstack(&s);
    push(&s,2);
    printstack(&s);
    push(&s,3);
    printstack(&s);
    push(&s,4);
    printstack(&s);
    push(&s,5);
    printstack(&s);
    push(&s,6);
    printstack(&s);
    push(&s,7);
    printstack(&s);
    push(&s,8);
    printstack(&s);
    push(&s,9);
    printstack(&s);
    push(&s,10);
    printstack(&s);
    push(&s,11);
    printstack(&s);
    push(&s,12);
    printstack(&s);
    int val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printstack(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    val=pop(&s);
    printf("pop %d\n",val);
    printstack(&s);
}