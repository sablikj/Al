#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -------------- Structures --------------//

typedef int width;

typedef struct _TreeNode {
    int value;
    struct _TreeNode *left;
    struct _TreeNode *right;
} TreeNode;

typedef struct _Tree {
    TreeNode *root;
    unsigned int itemsCount;
    unsigned int levels;
} Tree;

// ------------- Functions ----------------//

int getHeight (TreeNode* node);
int bstWidth(TreeNode* node, int t);
void Print_Node(TreeNode* nod);
void getLevelWidth(TreeNode* node, int currentLevel, int level, int *width);
int getMaxWidth(Tree* t, TreeNode* root);
#endif // TREE_H
