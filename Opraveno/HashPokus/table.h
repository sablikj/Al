#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 255

// Structures ---------------------------------------------------------//

typedef char Key[BUFFSIZE];

typedef struct _Data{
    unsigned int id;
    uint16_t timeout;
    bool retry;
    char command[BUFFSIZE];
}Data;

typedef struct _HashTableNode{
    Key key;
    Data* data;
    struct _HashTableNode* next;
}HashTableNode;

typedef struct _HashTable{
    HashTableNode** buckets;
    unsigned int size;
    unsigned int count;
}HashTable;

typedef void (*NodeProc)(unsigned int index, HashTableNode* node);

// Functions ---------------------------------------------------------//

unsigned int Hash(HashTable* table, Key key);
HashTable* HashTable_Init(unsigned int size);
bool HashTable_Insert(HashTable* table, Key key, Data* data);
void ProcessFunction(unsigned int index, HashTableNode* node);
void HashTable_Process(HashTable* table, NodeProc proc);
void HashTable_Print(HashTable* table);
void ProcessFunction_PrintNodes();

#endif // TABLE_H
