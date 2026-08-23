#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode
{
    int *data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
}BiTNode,*BiTree;

// 队列结构定义（用于层次遍历）
#define MAX_QUEUE_SIZE 100  // 队列最大容量

typedef struct Queue
{
    BiTree data[MAX_QUEUE_SIZE];  // 存储二叉树节点指针
    int front;  // 队头指针
    int rear;   // 队尾指针
}Queue;

// 初始化队列
void initQueue(Queue *q)
{
    q->front = 0;
    q->rear = 0;
}

// 判断队列是否为空
int isEmpty(Queue *q)
{
    return q->front == q->rear;
}

// 入队操作
int enqueue(Queue *q, BiTree node)
{
    if ((q->rear + 1) % MAX_QUEUE_SIZE == q->front)
    {
        printf("队列已满！\n");
        return 0;
    }
    q->data[q->rear] = node;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    return 1;
}

// 出队操作
BiTree dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        printf("队列为空！\n");
        return NULL;
    }
    BiTree node = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return node;
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

// 层次遍历（广度优先遍历）
void levelOrder(BiTree root)
{
    if (root == NULL) return;
    
    Queue q;
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













