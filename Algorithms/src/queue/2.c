#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define Maxsize 50

typedef struct{
    int data[Maxsize];
    int front,rear;
}SqQueue;

void InitQueue(SqQueue* Q)
{
    Q->rear=Q->front=0;
}

bool QueueEmpty(SqQueue* Q)
{
    if(Q->front==Q->rear)
    {
        return true;
    }
    else {
    return false;
    }
}
bool QueueFull(SqQueue* Q)
{
    if((Q->rear+1)%Maxsize==Q->front)
    {
        return true;
    }
    else {
    return false;
    }
}

bool enqueue(SqQueue* Q,int x)
{
    if(QueueFull(Q)==true)
    {
        return false;
    }
    Q->data[Q->rear]=x;
    Q->rear=(Q->rear+1)%Maxsize;
    return true;
}

int dequeue(SqQueue* Q)
{
    if(QueueEmpty(Q)==true)
    {
        return false;
    }
    int x=Q->data[Q->front];
    Q->front=(Q->front+1)%Maxsize;
    return x;
}

int main()
{
    SqQueue q;
    InitQueue(&q);
    enqueue(&q,'a');
    enqueue(&q,'b');
    enqueue(&q,'c');
    enqueue(&q,'d');
    enqueue(&q,'e');
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    return 0;
}
