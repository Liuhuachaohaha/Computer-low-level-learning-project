#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct LNode
{
    int data;
    struct LNode *link;
}LNode;

bool initlist(LNode** L)
{
    *L=(LNode*)malloc(sizeof(LNode));
    if(*L==NULL) return false;
    (*L)->link=NULL;
    (*L)->data=0;   // 头结点data存表长
    return true;
}

/* ======== 调试辅助函数 ======== */
LNode* GetElem(LNode* L, int i)
{
    if(L==NULL || i<0) return NULL;
    LNode *p=L;
    int j=0;
    while(p!=NULL && j<i) { p=p->link; j++; }
    return p;
}

bool ListInsert(LNode* L, int i, int e)
{
    if(L==NULL || i<1) return false;
    LNode *p = GetElem(L, i-1);
    if(p==NULL) return false;
    LNode *s = (LNode*)malloc(sizeof(LNode));
    if(s==NULL) return false;
    s->data = e;
    s->link = p->link;
    p->link = s;
    L->data++;
    return true;
}

void PrintList(LNode* L)
{
    if(L==NULL) { printf("[链表空或NULL]\n"); return; }
    printf("链表数量：%d 表头",L->data);
    LNode *p = L->link;
    while(p != NULL) {
        printf(" -> [%d]", p->data);
        p = p->link;
    }
    printf(" -> NULL\n");
}

void func(LNode* L,int n)
{
    int* q;
    q=(int*)malloc(sizeof(int)*(n+1));
    for(int i=0;i<n+1;i++)
    {
        *(q+i)=0;
    }
    LNode* r;
    LNode* count=L;
    while(L->link!=NULL)
    {
        int m=abs(L->link->data);
        if(*(q+m)==0)
        {
            *(q+m)=1;
            L=L->link;
        }
        else 
        {
            r=L->link;
            L->link=r->link;
            free(r);
            count->data--;
        }
    }
    free(q);
}

int main()
{
    LNode* L;
    bool ok=initlist(&L);
    printf("初始化: %s\n", ok ? "成功" : "失败");
    if(!ok) return 1;

    /* ===== 408 2015原题典型测试用例：包含多个绝对值重复 =====
       输入: 21, -15, 15, 7, 15
       正确输出(删重后): 21, -15, 7  （第二个15被删，第三个15也被删）
    */
    int test_data[] = {21, -15, 15, 7, 15};
    int len = sizeof(test_data)/sizeof(test_data[0]);
    printf("插入测试数据: ");
    for(int i=0; i<len; i++) {
        ListInsert(L, i+1, test_data[i]);
        printf("%d ", test_data[i]);
    }
    printf("\n");
    printf("插入完成: "); PrintList(L);

    // 调用算法：n 取 >= max|data| 的值即可，这里取 100
    func(L, 100);
    PrintList(L);
    return 0;
}