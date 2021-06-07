#include "tree.h"

bool Tree_Init(Tree* tree){
    if(tree == NULL){
        return false;
    }
    tree->root = NULL;
    tree->count = 0;
    tree->levels = 0;

    printf("Tree successfully initialized.\n");
    return true;
}
void Delete_Node(TreeNode* node){
    if(node == NULL){
        return;
    }
    Delete_Node(node->left);
    Delete_Node(node->right);

    free(node);
    return;
}
void Tree_Clear(Tree* tree){
    if(tree == NULL || tree->root == NULL){
        return;
    }
    Delete_Node(tree->root);
    tree->root = NULL;
    tree->count = 0;
    tree->levels = 0;
    printf("All nodes were removed.\n");
}

int getHeight(TreeNode* node){
    if(node == NULL){
        return -1;
    }
    int leftH, rightH;

    leftH = getHeight(node->left);
    rightH = getHeight(node->right);

    if(leftH >= rightH){
        return leftH + 1;
    }
    else{
        return rightH + 1;
    }
}

int Tree_Height(Tree* tree){
    if(tree == NULL || tree->root == NULL){
        return -1;
    }
    return printf("Tree height is: %d\n", getHeight(tree->root));
}

bool Tree_Insert(Tree* tree, int value){
    if(tree == NULL){
        return false;
    }

    // First node
    if(tree->count == 0){
        TreeNode* node = malloc(sizeof (TreeNode));
        if(node == NULL){
            return false;
        }
        node->value = value;
        node->left = NULL;
        node->right = NULL;

        tree->root = node;
        tree->count++;
        tree->levels++;

        printf("Node successfully inserted.\n");
        return true;
    }
    else{
        TreeNode* next = tree->root;
        TreeNode* last = NULL;

        while(next != NULL){
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
        TreeNode* node = malloc(sizeof (TreeNode));
        if(node == NULL){
            return false;
        }
        node->value = value;
        node->left = NULL;
        node->right = NULL;

        if(value < last->value){
            last->left = node;
        }
        else{
            last->right = node;
        }
        tree->count++;
        tree->levels = Tree_Height(tree);
        printf("Node successfully inserted.\n");
        return true;
    }
}

int Tree_maxWidth(Tree* tree){
    if(tree == NULL){
        return 0;
    }
    return printf("Max width is: %d\n",getMaxWidth(tree,tree->root));
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

int getMaxWidth(Tree* tree, TreeNode* node){
  int width, maxWidth = -1;
  for(unsigned int i = 0; i < tree->levels; i++){
     width = 0;
     getLevelWidth(node, 0, i, &width);
     maxWidth = width > maxWidth ? width : maxWidth;
 }
 return maxWidth;
}

bool Tree_FindNode(Tree* tree, int value){
    if(tree == NULL || tree->root == NULL){
        return false;
    }
    TreeNode* node = tree->root;
    while(node != NULL){
        if(value < node->value){
            node = node->left;
        }
        if(value > node->value){
            node = node->right;
        }
        else{
            printf("Node was found!\n");
            return true;
        }
    }
    return printf("Node wasn't found.\n");
}

int Tree_GetCount(Tree* tree){
    if(tree == NULL || tree->root == NULL){
        return -1;
    }
    return printf("# of nodes in tree: %d\n",tree->count);
}

void Tree_Process(Tree tree, TreeNodeProc proc, TreeProcessMode mode){
    switch (mode){
      case procPREORDER:
        print_preorder(tree.root, proc);
        break;
      case procINORDER:
        print_inorder(tree.root, proc);
        break;
      case procPOSTORDER:
        print_postorder(tree.root, proc);
        break;
    }
}

void print_preorder(TreeNode* node, TreeNodeProc proc){
  if(node == NULL){
      return;
  }
  (*proc)(node);
  print_preorder(node->left, proc);
  print_preorder(node->right, proc);
}

void print_inorder(TreeNode* node, TreeNodeProc proc){
    if(node == NULL){
        return;
    }
    print_inorder(node->left, proc);
    (*proc)(node);
    print_inorder(node->right, proc);
}

void print_postorder(TreeNode* node, TreeNodeProc proc){
    if(node == NULL){
        return;
    }
    print_postorder(node->left, proc);
    print_postorder(node->right, proc);
    (*proc)(node);
}

void Print_Node(TreeNode* node) {
    if(node == NULL){
        return;
    }
    printf("%d ",node->value);
    Print_Node(node->left);
    printf("\n");
    Print_Node(node->right);
}

void Tree_Print(Tree* tree){
    if(tree == NULL || tree->root == NULL){
        return;
    }
    Print_Node(tree->root);
}
