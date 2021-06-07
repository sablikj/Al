#include <stdio.h>
#include "table.h"

int main()
{
    printf("Hash table v1.0!\n");

    Key k1 = "Cache";
    Key k2 = "Data";
    Key k3 = "SmallCache";
    Key k4 = "NotData";

    Data d1 = {0,5,false,"com"};
    Data d2 = {1,10,true,"com"};
    Data d3 = {2,15,false,"com"};
    Data d4 = {3,20,true,"com"};

    HashTable *table = HashTable_Init(2);
    HashTable_Insert(table, k1, &d1);
    HashTable_Insert(table, k2, &d2);
    HashTable_Print(table);
    HashTable_Insert(table, k3, &d3);
    HashTable_Insert(table, k4, &d4);
    HashTable_Print(table);
    HashTable_Process(table,ProcessFunction);

    return 0;
}
