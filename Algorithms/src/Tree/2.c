#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode
{
    int *data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
}BiTNode,*BiTree;

// 链式队列节点定义（每个节点存储一棵二叉树节点指针）
typedef struct QNode
{
    BiTree data;        // 队列元素：二叉树节点指针
    struct QNode *next; // 下一个队列节点
}QNode;

// 链式队列结构（用队头队尾指针实现，无需固定容量）
typedef struct LinkQueue
{
    QNode *front;   // 队头指针
    QNode *rear;    // 队尾指针
}LinkQueue;

// 初始化链式队列（带头节点，方便统一出队操作）
void initQueue(LinkQueue *q)
{
    q->front = (QNode*)malloc(sizeof(QNode)); // 分配头节点
    q->front->next = NULL;
    q->rear = q->front; // 初始时队头队尾都指向头节点
}

// 判断队列是否为空
int isEmpty(LinkQueue *q)
{
    return q->front == q->rear;
}

// 入队操作（队尾插入）
void enqueue(LinkQueue *q, BiTree node)
{
    QNode *p = (QNode*)malloc(sizeof(QNode));
    p->data = node;
    p->next = NULL;
    q->rear->next = p; // 新节点接到队尾
    q->rear = p;       // 队尾指针后移
}

// 出队操作（队头删除）
BiTree dequeue(LinkQueue *q)
{
    if (isEmpty(q))
    {
        printf("队列为空！\n");
        return NULL;
    }
    QNode *p = q->front->next; // 第一个有效节点（跳过头节点）
    BiTree node = p->data;
    q->front->next = p->next;  // 摘下该节点
    if (q->rear == p)          // 若出队的是最后一个有效节点，队尾回退到头节点
        q->rear = q->front;
    free(p);
    return node;
}

// 销毁队列（释放所有节点，包括头节点）
void destroyQueue(LinkQueue *q)
{
    QNode *p = q->front;
    while (p != NULL)
    {
        QNode *tmp = p;
        p = p->next;
        free(tmp);
    }
    q->front = q->rear = NULL;
}

BiTNode* createNode(int val)
{
    BiTNode* node=(BiTNode*)malloc(sizeof(BiTNode));
    node->data=(int*)malloc(sizeof(int));
    node->data[0]=val;
    node->lchild=NULL;
    node->rchild=NULL;
    return node;
}

BiTree buildTestTree()
{
    BiTNode* root = createNode(1);
    root->lchild = createNode(2);
    root->rchild = createNode(3);
    root->lchild->lchild = createNode(4);
    root->lchild->rchild = createNode(5);
    root->lchild->lchild->lchild= createNode(6);
    root->lchild->lchild->rchild= createNode(7);
    root->lchild->rchild->lchild = createNode(8);
    root->lchild->rchild->rchild = createNode(9);
    return root;
}

// 前序遍历：根 → 左 → 右
void preOrder(BiTree root)
{
    if (root == NULL) return;
    printf("%d ", *(root->data));   // 访问根节点
    preOrder(root->lchild);          // 递归左子树
    preOrder(root->rchild);          // 递归右子树
}
// 中序遍历：左 → 根 → 右
void inOrder(BiTree root)
{
    if (root == NULL) return;
    inOrder(root->lchild);           // 递归左子树
    printf("%d ", *(root->data));   // 访问根节点
    inOrder(root->rchild);           // 递归右子树
}
// 后序遍历：左 → 右 → 根
void postOrder(BiTree root)
{
    if (root == NULL) return;
    postOrder(root->lchild);         // 递归左子树
    postOrder(root->rchild);         // 递归右子树
    printf("%d ", *(root->data));   // 访问根节点
}

// 层次遍历（广度优先遍历）—— 使用链式队列
void levelOrder(BiTree root)
{
    if (root == NULL) return;

    LinkQueue q;
    initQueue(&q);
    enqueue(&q, root);  // 根节点入队

    while (!isEmpty(&q))
    {
        BiTree node = dequeue(&q);  // 出队
        printf("%d ", *(node->data));  // 访问节点

        if (node->lchild != NULL)   // 左孩子入队
            enqueue(&q, node->lchild);
        if (node->rchild != NULL)   // 右孩子入队
            enqueue(&q, node->rchild);
    }
    destroyQueue(&q);  // 释放队列内存
}

void destroyTree(BiTree root)
{
    if (root == NULL) return;
    destroyTree(root->lchild);
    destroyTree(root->rchild);
    free(root->data);
    free(root);
}
int main()
{
    BiTree root = buildTestTree();
    printf("二叉树构建完成！根节点值：%d\n", *(root->data));
    printf("根节点左孩子：%d\n", *(root->lchild->data));
    printf("根节点右孩子：%d\n", *(root->rchild->data));
    printf("根节点左孩子的左孩子：%d\n", *(root->lchild->lchild->data));
    printf("根节点左孩子的左、右孩子：%d\n", *(root->lchild->rchild->data));
    printf("前序遍历：");
    preOrder(root);
    printf("\n");
    printf("中序遍历：");
    inOrder(root);
    printf("\n");
    printf("后序遍历：");
    postOrder(root);
    printf("\n");
    printf("层次遍历：");
    levelOrder(root);
    printf("\n");
    destroyTree(root);
    return 0;
}
