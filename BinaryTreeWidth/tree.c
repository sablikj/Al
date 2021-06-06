#include "tree.h"

int THeight(Tree * t){
    if(t == NULL){
        return 0;
    }
    return getHeight(t->root);
}

void PP(Tree* t){
    if(t == NULL){
        return;
    }
    Print_Node(t->root);
}

int TWidth(Tree * t){
    if(t == NULL){
        return 0;
    }
    return getMaxWidth(t,t->root);
}

void Print_Node(TreeNode *nod) {
    if(nod == NULL){
        return;
    }
    printf("%d ",nod->value);
    Print_Node(nod->left);
    printf("\n");
    Print_Node(nod->right);
}

int getHeight (TreeNode * node){
    int leftHeight, rightHeight;
    if(node == NULL){
        return 0;
    }
    leftHeight = getHeight(node->left);
    rightHeight = getHeight(node->right);

    if(leftHeight>=rightHeight){
        return leftHeight + 1;
    }else{
        return rightHeight + 1;
    }
}

void getLevelWidth(TreeNode* node, int currentLevel, int level, int *width) {
  if(node == NULL){
      return;
  }
  if(currentLevel == level){
      *width = *width + 1;
      return;
  }
  getLevelWidth(node->left, currentLevel+1, level, width);
  getLevelWidth(node->right, currentLevel+1, level, width);
}

int getMaxWidth(Tree* t, TreeNode* node) {
    int width, maxWidth = -1;
 for(unsigned int i = 0; i < t->levels ; i++){
     width = 0;
     getLevelWidth(node, 0, i, &width);
     maxWidth = width > maxWidth ? width : maxWidth;
 }

 return maxWidth;
}

bool Tree_Init(Tree* const root) {
    if(root == NULL){
        return false;
    }
    root->itemsCount = 0;
    root->root = NULL;
    return true;
}

bool Tree_Insert(Tree* tree, int value) {
    if(tree==NULL){
        return false;
    }

    if(tree->itemsCount == 0){
        TreeNode * node = malloc(sizeof (TreeNode));
        if(node==NULL){
            return false;
        }

        node->value = value;
        node->left = NULL;
        node->right = NULL;
        tree->root = node;
        tree->itemsCount++;
        return true;

    }else{
        TreeNode* next = tree->root;
        TreeNode* last = NULL;

        while (next != NULL) {
            if(next->value == value){return false;}
            if(next->value < value){
                last = next;
                next = next->right;
            }
            else{
                last = next;
                next = next->left;
            }
        }

        TreeNode * node = malloc(sizeof (TreeNode));
        if(node==NULL){
            return false;
        }
        node->value = value;
        node->left = NULL;
        node->right = NULL;

        if(last->value > value){
            last->left = node;
        }
        else{
            last->right = node;
        }
        tree->itemsCount ++;
        tree->levels = THeight(tree);
        return true;
    }
}
