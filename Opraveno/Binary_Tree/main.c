#include <stdio.h>
#include "tree.h"

int main()
{
    printf("Binary Tree v1.0\n");
    Tree *tree = malloc(sizeof(Tree));
    Tree_Init(tree);
    Tree_Insert(tree,5);
    Tree_Insert(tree,3);
    Tree_Insert(tree,2);
    Tree_Insert(tree,10);
    Tree_GetCount(tree);
    Tree_FindNode(tree,2);
    Tree_Print(tree);
    Tree_maxWidth(tree);
    Tree_Clear(tree);
    return 0;
}
