
#include <stdio.h>
#include <stdlib.h>

#define Link 0
#define Thread 1

typedef  struct BiTNode
{
    int *data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
    int ltag;
    int rtag;
}BiTNode,*BiTree;

BiTNode* createNode(int val)
{
    BiTNode* node=(BiTNode*)malloc(sizeof(BiTNode));
    node->data=(int*)malloc(sizeof(int));
    *(node->data)=val;
    node->lchild=NULL;
    node->rchild=NULL;
    node->ltag=Link;
    node->rtag=Link;
    return node;
}

BiTNode* createTree(int val,BiTree left,BiTree right)
{
    BiTNode* node=createNode(val);
    node->lchild=left;
    node->rchild=right;
    return node;
}

BiTree buildTestTree(void)
{
    return createTree(9,
        createTree(4, 
        createTree(2, createNode(1), createNode(3)), 
        createTree(7, createNode(5), createNode(8))),
        createNode(10));
}

void inOrder(BiTree root)
{
    if (root == NULL) return;
    if(root->ltag==Link){inOrder(root->lchild);}    // 递归左子树
    printf("%d ", *(root->data));   // 访问根节点
    if(root->rtag==Link){inOrder(root->rchild);}         // 递归右子树
}

void InThread(BiTree p,BiTree *pre)
{
    if(p==NULL) return;
    InThread(p->lchild, pre);
     
    if(p->lchild==NULL){p->ltag=Thread;p->lchild=*pre;}
    if(*pre!=NULL && (*pre)->rchild==NULL)
    {(*pre)->rtag=Thread;(*pre)->rchild=p;}
    *pre=p;
    InThread(p->rchild, pre);
}

void createInThread(BiTree root)
{
    BiTree pre=NULL;
    if(root!=NULL)
    {
        InThread(root, &pre);
        pre->rchild=NULL;
        pre->rtag=Thread;
    }
}

BiTNode* firstNode(BiTree p)
{
    if(!p) return NULL;
    while(p->ltag==Link) p=p->lchild;
    return p;
}

BiTNode* lastNode(BiTree p)
{
    if(!p) return NULL;
    while(p->rtag==Link) p=p->rchild;
    return p;
}

BiTNode* nextNode(BiTree p)
{
    if(!p) return NULL;
    return (p->rtag == Thread) ? p->rchild : firstNode(p->rchild);
}

BiTNode* preNode(BiTree p)
{
    if (!p) return NULL;
    return (p->ltag == Thread) ? p->lchild : lastNode(p->lchild);
}

void inOrder_thread(BiTree T)
{
    for(BiTree p=firstNode(T);p!=NULL;p=nextNode(p))
    {printf("%d ", *(p->data)); }
}

void ReinOrder_thread(BiTree T)
{
    for(BiTree p=lastNode(T);p!=NULL;p=preNode(p))
    {printf("%d ", *(p->data)); }
}

void insertRight(BiTree p,BiTree s)
{
    s->lchild=p;s->ltag=Thread;
    s->rchild=p->rchild;s->rtag=p->rtag;
    p->rchild=s;p->rtag=Link;
    if(s->rtag==Link){firstNode(s->rchild)->lchild=s;}
}

void insertLeft(BiTree p, BiTree s)
{
    s->rchild = p;          s->rtag = Thread;
    s->lchild = p->lchild;  s->ltag = p->ltag;   // inherit
    p->lchild = s;          p->ltag = Link;
    if (s->ltag == Link) lastNode(s->lchild)->rchild = s;    // fix predecessor's right thread
}

BiTNode* search(BiTree root, int val)
{
    for (BiTree p = firstNode(root); p != NULL; p = nextNode(p))
    {if (*(p->data) == val) return p;}
    return NULL;
}


BiTNode* findParent(BiTree root,BiTree target)
{
    if (root == NULL || target == NULL || root == target) return NULL;
    for (BiTree p = firstNode(root); p != NULL; p = nextNode(p))
    {
        if ((p->ltag == Link && p->lchild == target) ||
            (p->rtag == Link && p->rchild == target))
            return p;
    }
    return NULL;
}



int main(void)
{
    BiTree root = buildTestTree(); 
    printf("inoder:");inOrder(root);printf("\n");
    createInThread(root);
    printf("inoder_thread:");inOrder_thread(root);printf("\n");
    printf("Reinoder_thread:");ReinOrder_thread(root);printf("\n");

    insertRight(search(root, 5), createNode(6));
    printf("After insert 10 under 5: ");
    inOrder_thread(root); printf("\n");

    
    printf("After delete 10: ");
    inOrder_thread(root); printf("\n");
    return 0;
}