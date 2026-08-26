#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct LNode{
    int data;
    struct LNode *next;
}LNode,*Linklist;

bool initlist(LNode** L)
{
    *L=(LNode*)malloc(sizeof(LNode));
    if(*L==NULL) return false;
    (*L)->next=NULL;
    (*L)->data=0;   // 头结点data可存表长
    return true;
}

LNode *GetElem(LNode* L,int i)
{
    if(L==NULL || i<0) return NULL;
    LNode *p=L;
    int j=0;
    while(p!=NULL && j<i)
    {
        p=p->next;
        j++;
    }
    return p;
}

// 在第i个位置插入元素e（带头结点，i从1开始）
bool ListInsert(LNode* L, int i, int e)
{
    if(L==NULL || i<1) return false;
    LNode *p = GetElem(L, i-1);  // 找到第i-1个节点（前驱）
    if(p==NULL) return false;
    LNode *s = (LNode*)malloc(sizeof(LNode));
    if(s==NULL) return false;
    s->data = e;
    s->next = p->next;
    p->next = s;
    L->data++;  // 表长+1
    return true;
}

int main()
{
    LNode* L;                        // 声明为指针（头指针）
    bool ok = initlist(&L);          // 传入指针地址，malloc结果写回L
    printf("初始化: %s\n", ok ? "成功" : "失败");
    if(!ok) return 1;

    // 插入测试数据
    ListInsert(L, 1, 100);
    ListInsert(L, 2, 200);
    ListInsert(L, 3, 300);

    // 打印第0个位置（头结点）的数据（存的是表长）
    LNode *p0 = GetElem(L, 0);
    if(p0) printf("GetElem(L,0)->data = 表长 = %d\n", p0->data);

    // 打印第1个节点的数据
    LNode *p1 = GetElem(L, 1);
    if(p1) printf("GetElem(L,1)->data = %d\n", p1->data);

    // 遍历全部
    printf("\n遍历链表: ");
    for(int i=1; i<=L->data; i++) {
        LNode *p = GetElem(L, i);
        if(p) printf("[节点%d: %d] ", i, p->data);
    }
    printf("\n");
    ListInsert(L, 2, 400);
    // 遍历全部
    printf("\n遍历链表: ");
    for(int i=1; i<=L->data; i++) {
        LNode *p = GetElem(L, i);
        if(p) printf("[节点%d: %d] ", i, p->data);
    }
    printf("\n");
    return 0;
}