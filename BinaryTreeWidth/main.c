#include <stdio.h>
#include "tree.h"

int main()
{
    Tree stromis;
    Tree_Init(&stromis);
    Tree_Insert(&stromis,10);
    Tree_Insert(&stromis,7);
    Tree_Insert(&stromis,11);
    Tree_Insert(&stromis,8);
    Tree_Insert(&stromis,20);
    Tree_Insert(&stromis,12);
    Tree_Insert(&stromis,5);
    PP(&stromis);
    TWidth(&stromis);
    printf(" najsirsia vetva %d\n",TWidth(&stromis));
    Tree_Insert(&stromis,25);
    Tree_Insert(&stromis,23);

    return 0;
}
