#include <stdio.h>
#include <stdlib.h>
#include "table.h"

void menu(){
    printf("1 - HashTable_Init\n");
    printf("2 - HashTable_Insert\n");
    printf("3 - HashTable_Print\n");
    printf("E - END\n");
    printf("M - MENU\n");
}

int main()
{
    Key k = "Cache";
    Key k1 = "Data";
    Key k2 = "SmallCache";
    Key k3 = "NotData";
    Data h = {0,10,true,"its"};
    Data h1 = {1,12,true,"time"};
    Data h2 = {2,13,true,"to"};
    Data h3 = {3,19,true,"stop"};

    printf("Hash Table v1.0\n\n");

    HashTable *table = HashTable_Init(2);
    HashTable_Insert(table,k,&h);
    HashTable_Insert(table,k1,&h1);
    HashTable_Print(table);
    HashTable_Insert(table,k2,&h2);
    HashTable_Insert(table,k3,&h3);
    HashTable_Print(table);
    HashTable_Process(table, ProcessFunction);



    return 0;
}
