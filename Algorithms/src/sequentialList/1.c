#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

typedef struct{
    int* data;
    int len;
    int cap;
}seqlist;

void initlist(seqlist* L)
{
    L->data=(int*)malloc(MAXSIZE*sizeof(int));
    L->len=0;
    L->cap=MAXSIZE;
}

int insertlist(seqlist* L,int pos,int val)
{
    if(pos<1 || pos>L->len+1)
    {
        printf("insert position invalid\n");
        return -1;
    }
    if(L->len>=L->cap)
    {
        printf("list is full");
        return -1;
    }
    for(int i=L->len;i>=pos;i--)
    {
        L->data[i]=L->data[i-1];
    }
    L->data[pos-1]=val;
    L->len++;
    return 0;
}

int deletelist(seqlist* L,int pos,int *val)
{
    if(pos<1 || pos>L->len)
    {
        printf("delete position invalid");
        return -1;
    }
    *val=L->data[pos-1];
    for(int i=pos;i<L->len;i++)
    {
        L->data[i-1]=L->data[i];
    }
    L->len--;
    return 0;
}

void printList(seqlist* L)
{
    printf("list: ");
    for(int i=0;i<L->len;i++)
    {
        printf("%d ",L->data[i]);
    }
    printf(" length=%d capacity=%d\n",L->len,L->cap);
}

int main()
{
    seqlist L;
    initlist(&L);
    printList(&L);
    insertlist(&L,1,1);
    printList(&L);
    insertlist(&L,2,2);
    printList(&L);
    insertlist(&L,2,3);
    printList(&L);
    int val;
    deletelist(&L,2,&val);
    printList(&L);
}