#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

//---------------------- Structures ------------------------------//

typedef char Key[255];

typedef struct _Data{
    unsigned int id;
    uint16_t timeout;
    bool retry;
    char command[255];
}Data ;

typedef struct _HashTableNode{
    Key key;
    Data *data;
    struct _HashTableNode *next;
}HashTableNode;

typedef struct _HashTable{
    HashTableNode **buckets;
    unsigned int count;
    unsigned int size;
}HashTable;

typedef void (*NodeProc)(unsigned int index, HashTableNode *node);

//----------------------- Functions ------------------------------//

unsigned int Hash(HashTable *table, Key key);
HashTable *HashTable_Init(unsigned int size);
bool HashTable_Insert(HashTable *table, Key key, Data *data);
void ProcessFunction(Key key, HashTableNode *node);
void HashTable_Process(HashTable *table, NodeProc proc);
void HashTable_Print(HashTable *table);

#endif // TABLE_H
