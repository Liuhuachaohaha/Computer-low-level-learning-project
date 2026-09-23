#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 表达式树节点：叶子是操作数(数字)，非叶子是运算符(+ - * /)
typedef struct ExpNode
{
    int isOp;          // 1: 运算符  0: 操作数
    char op;           // 运算符
    int val;           // 操作数值
    struct ExpNode *lchild;
    struct ExpNode *rchild;
} ExpNode, *ExpTree;

// 创建运算符节点
ExpNode* createOpNode(char op, ExpNode *left, ExpNode *right)
{
    ExpNode *node = (ExpNode*)malloc(sizeof(ExpNode));
    node->isOp = 1;
    node->op = op;
    node->lchild = left;
    node->rchild = right;
    return node;
}

// 创建操作数节点
ExpNode* createValNode(int val)
{
    ExpNode *node = (ExpNode*)malloc(sizeof(ExpNode));
    node->isOp = 0;
    node->val = val;
    node->lchild = NULL;
    node->rchild = NULL;
    return node;
}

// 最简版 LNR（中序遍历）输出中缀表达式
// 规则：遇到运算符节点，就在其前后加一对括号：( 左子树 运算符 右子树 )
void inOrderExpr(ExpTree root)
{
    if (root == NULL) return;

    if (root->isOp) printf("(");        // 运算符节点前加左括号
    inOrderExpr(root->lchild);          // 左
    if (root->isOp)
        printf(" %c ", root->op);       // 根（运算符）
    else
        printf("%d", root->val);        // 根（操作数）
    inOrderExpr(root->rchild);          // 右
    if (root->isOp) printf(")");        // 运算符节点后加右括号
}

// 销毁表达式树
void destroyTree(ExpTree root)
{
    if (root == NULL) return;
    destroyTree(root->lchild);
    destroyTree(root->rchild);
    free(root);
}

// 计算表达式树的值（验证表达式正确性）
int evalTree(ExpTree root)
{
    if (root == NULL) return 0;
    if (!root->isOp) return root->val;

    int left = evalTree(root->lchild);
    int right = evalTree(root->rchild);
    switch (root->op)
    {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return right != 0 ? left / right : 0;
    }
    return 0;
}

int main()
{
    // 构建表达式树：表示 ((3 + 5) * 2) - (8 / 4)
    //         -
    //        / \
    //       *   /
    //      / \ / \
    //     +  2 8  4
    //    / \
    //   3   5
    ExpTree root = createOpNode('-',
        createOpNode('*',
            createOpNode('+', createValNode(3), createValNode(5)),
            createValNode(2)),
        createOpNode('/', createValNode(8), createValNode(4)));

    printf("中序遍历(LNR)得到的中缀表达式：\n");
    inOrderExpr(root);
    printf("\n");

    printf("表达式计算结果：%d\n", evalTree(root));
    // 预期：((3+5)*2) - (8/4) = 16 - 2 = 14

    destroyTree(root);
    return 0;
}
