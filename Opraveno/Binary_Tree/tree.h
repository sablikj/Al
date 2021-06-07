#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structures -------------------------------------------------------------//

typedef struct _TreeNode{
    int value;
    struct _TreeNode* left;
    struct _TreeNode* right;
}TreeNode ;

typedef struct _Tree{
    TreeNode* root;
    unsigned int count;
    unsigned int levels;
}Tree ;

typedef void (*TreeNodeProc)(TreeNode* node) ;

typedef enum _TreeProcessMode{
    procPREORDER = 1,
    procINORDER = 2,
    procPOSTORDER = 3
}TreeProcessMode;

// Functions -------------------------------------------------------------//

bool Tree_Init(Tree* tree);
void Tree_Clear(Tree* tree);
bool Tree_Insert(Tree* tree, int value);
bool Tree_FindNode(Tree* tree, int value);
int Tree_GetCount(Tree* tree);
void Tree_Process(Tree root, TreeNodeProc proc, TreeProcessMode mode);

int Tree_maxWidth(Tree* tree);
void getLevelWidth(TreeNode* node, int currentLevel, int level, int *width);
int getMaxWidth(Tree* tree, TreeNode* node);

void Print_Node(TreeNode* node);
void Tree_Print(Tree* tree);

void print_preorder(TreeNode* node, TreeNodeProc proc);
void print_inorder(TreeNode* node, TreeNodeProc proc);
void print_postorder(TreeNode* node, TreeNodeProc proc);

#endif // TREE_H
