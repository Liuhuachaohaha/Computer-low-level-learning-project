# In-order Threaded Binary Tree — Complete Guide

> Tree construction follows the bottom-up approach of `3.c` (`createTree(val, left, right)`).

## Table of Contents
1. [Overview](#1-overview)
2. [Data Structure Definition](#2-data-structure-definition)
3. [Tree Construction (Bottom-up)](#3-tree-construction-bottom-up)
4. [In-order Threading](#4-in-order-threading)
5. [Lookup Functions](#5-lookup-functions)
6. [Thread-based In-order Traversal](#6-thread-based-in-order-traversal)
7. [Insertion](#7-insertion)
8. [Deletion](#8-deletion)
9. [Search & Destroy](#9-search--destroy)
10. [Full `main` Demonstration](#10-full-main-demonstration)
11. [Complexity Summary](#11-complexity-summary)

---

## 1. Overview

An **in-order threaded binary tree** (中序线索二叉树) stores predecessor/successor pointers in empty child slots, enabling O(1) average successor/predecessor lookup and stack-free in-order traversal.

| Tag | Meaning |
|-----|---------|
| `0` (Link) | Pointer is a real child |
| `1` (Thread) | Pointer is a predecessor / successor thread |

---

## 2. Data Structure Definition

```c
#include <stdio.h>
#include <stdlib.h>

#define Link   0
#define Thread 1

typedef struct BiTNode
{
    int *data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
    int ltag;       // 0 = Link, 1 = Thread
    int rtag;       // 0 = Link, 1 = Thread
} BiTNode, *BiTree;
```

Tags default to `Link` during construction and are set to `Thread` during the threading pass.

---

## 3. Tree Construction (Bottom-up)

Following the style of `3.c`, child subtrees are built first and passed into the parent constructor.

### 3.1 `createNode(val)` — Leaf node

```c
BiTNode* createNode(int val)
{
    BiTNode* node = (BiTNode*)malloc(sizeof(BiTNode));
    node->data = (int*)malloc(sizeof(int));
    *(node->data) = val;
    node->lchild = NULL;
    node->rchild = NULL;
    node->ltag = Link;
    node->rtag = Link;
    return node;
}
```

### 3.2 `createTree(val, left, right)` — Internal node with children

```c
BiTNode* createTree(int val, BiTree left, BiTree right)
{
    BiTNode* node = createNode(val);
    node->lchild = left;
    node->rchild = right;
    return node;
}
```

### 3.3 `buildTestTree()` — Assemble bottom-up

```c
BiTree buildTestTree(void)
{
    return createTree(1,
        createTree(2,
            createTree(4, createNode(6), createNode(7)),
            createTree(5, createNode(8), createNode(9))),
        createNode(3));
}
```

This produces:
```
        1
       / \
      2   3
     / \
    4   5
   / \ / \
  6  7 8  9
```

---

## 4. In-order Threading

A `pre` pointer tracks the previously visited node in in-order.

```c
void inThread(BiTree p, BiTree *pre)
{
    if (p == NULL) return;

    inThread(p->lchild, pre);                              // left subtree

    if (p->lchild == NULL) { p->ltag = Thread; p->lchild = *pre; }        // left thread
    if (*pre != NULL && (*pre)->rchild == NULL) { (*pre)->rtag = Thread; (*pre)->rchild = p; } // right thread of pre

    *pre = p;                                              // advance pre

    inThread(p->rchild, pre);                              // right subtree
}

void createInThread(BiTree root)
{
    BiTree pre = NULL;
    if (root != NULL)
    {
        inThread(root, &pre);
        pre->rtag   = Thread;    // last node: no successor
        pre->rchild = NULL;
    }
}
```

---

## 5. Lookup Functions

| Function | Logic |
|---|---|
| `firstNode(p)` | Walk left while `ltag == Link` |
| `lastNode(p)` | Walk right while `rtag == Link` |
| `nextNode(p)` | `rtag == Thread` → `rchild`; else `firstNode(rchild)` |
| `preNode(p)` | `ltag == Thread` → `lchild`; else `lastNode(lchild)` |

```c
BiTNode* firstNode(BiTree p)
{
    if (!p) return NULL;
    while (p->ltag == Link) p = p->lchild;
    return p;
}

BiTNode* lastNode(BiTree p)
{
    if (!p) return NULL;
    while (p->rtag == Link) p = p->rchild;
    return p;
}

BiTNode* nextNode(BiTree p)
{
    if (!p) return NULL;
    return (p->rtag == Thread) ? p->rchild : firstNode(p->rchild);
}

BiTNode* preNode(BiTree p)
{
    if (!p) return NULL;
    return (p->ltag == Thread) ? p->lchild : lastNode(p->lchild);
}
```

---

## 6. Thread-based In-order Traversal

No recursion, no stack.

```c
void inOrderThread(BiTree root)
{
    for (BiTree p = firstNode(root); p != NULL; p = nextNode(p))
        printf("%d ", *(p->data));
}
```

---

## 7. Insertion

Only the new node and its immediate predecessor/successor threads change — **no full re-threading**.

### `insertRight(p, s)`

```c
void insertRight(BiTree p, BiTree s)
{
    s->lchild = p;          s->ltag = Thread;
    s->rchild = p->rchild;  s->rtag = p->rtag;   // inherit
    p->rchild = s;          p->rtag = Link;
    if (s->rtag == Link) firstNode(s->rchild)->lchild = s;   // fix successor's left thread
}
```

### `insertLeft(p, s)`

```c
void insertLeft(BiTree p, BiTree s)
{
    s->rchild = p;          s->rtag = Thread;
    s->lchild = p->lchild;  s->ltag = p->ltag;   // inherit
    p->lchild = s;          p->ltag = Link;
    if (s->ltag == Link) lastNode(s->lchild)->rchild = s;    // fix predecessor's right thread
}
```

---

## 8. Deletion

Four cases based on how many real children `p` has:

| Case | Condition | Action |
|---|---|---|
| Leaf | both tags = Thread | `pred->rchild = p->rchild`, `succ->lchild = p->lchild` |
| Left only | `rtag == Thread` | `pred->rchild = p->lchild`, `lastNode(p->lchild)->rchild = p->rchild` |
| Right only | `ltag == Thread` | `succ->lchild = p->rchild`, `firstNode(p->rchild)->lchild = p->lchild` |
| Both | both tags = Link | Swap data with successor, then delete successor |

```c
void deleteNode(BiTree root, BiTree p)
{
    if (!p) return;
    BiTree succ = nextNode(p), pred = preNode(p);

    if (p->ltag == Thread && p->rtag == Thread) {
        if (pred) pred->rchild = p->rchild;
        if (succ) succ->lchild = p->lchild;
    } else if (p->rtag == Thread) {
        if (pred) pred->rchild = p->lchild;
        lastNode(p->lchild)->rchild = p->rchild;
    } else if (p->ltag == Thread) {
        if (succ) succ->lchild = p->rchild;
        firstNode(p->rchild)->lchild = p->lchild;
    } else {
        int tmp = *(p->data); *(p->data) = *(succ->data); *(succ->data) = tmp;
        deleteNode(root, succ);
        return;
    }
    free(p->data);
    free(p);
}
```

> In a full BST-style deletion you would also relink the parent's child pointer; this node design stores no parent link.

---

## 9. Search & Destroy

```c
BiTNode* search(BiTree root, int val)
{
    for (BiTree p = firstNode(root); p != NULL; p = nextNode(p))
        if (*(p->data) == val) return p;
    return NULL;
}

void destroyTree(BiTree root)
{
    BiTree p = firstNode(root);
    while (p != NULL)
    {
        BiTree next = nextNode(p);
        free(p->data);
        free(p);
        p = next;
    }
}
```

---

## 10. Full `main` Demonstration

```c
int main(void)
{
    BiTree root = buildTestTree();          // Phase 1: build
    createInThread(root);                   // Phase 2: thread

    printf("In-order (threaded): ");
    inOrderThread(root); printf("\n");      // expect: 6 4 7 2 8 5 9 1 3

    BiTree node4 = search(root, 4);
    if (node4) {
        printf("Node 4 predecessor: %d\n", *(preNode(node4)->data));   // 6
        printf("Node 4 successor  : %d\n", *(nextNode(node4)->data));  // 7
    }

    insertRight(search(root, 5), createNode(10));
    printf("After insert 10 under 5: ");
    inOrderThread(root); printf("\n");

    deleteNode(root, search(root, 8));
    printf("After delete 8: ");
    inOrderThread(root); printf("\n");

    destroyTree(root);
    return 0;
}
```

---

## 11. Complexity Summary

| Operation | Time | Space |
|---|---|---|
| Build tree (bottom-up) | O(n) | O(1) iterative / O(h) recursive |
| Thread tree | O(n) | O(h) recursion |
| First / last node | O(h) | O(1) |
| Successor / predecessor | O(1) avg, O(h) worst | O(1) |
| In-order traversal | O(n) | O(1) |
| Insert right / left | O(1) avg, O(h) worst | O(1) |
| Delete node | O(h) | O(1) |
| Search | O(n) | O(1) |

---

### Construction Workflow

```
createNode / createTree(val, left, right)   → build ordinary tree bottom-up
        ↓
buildTestTree()                              → assemble tree
        ↓
createInThread(root)                         → inThread() fills threads
        ↓
firstNode / lastNode / nextNode / preNode    → lookup
        ↓
inOrderThread / insert / delete / search     → operations
```

All insert/delete operations keep the tree valid **without full re-threading** — only adjacent predecessor/successor threads are adjusted.
