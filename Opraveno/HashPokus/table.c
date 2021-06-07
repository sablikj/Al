#include "table.h"

unsigned int Hash(HashTable* table, Key key){
    if(table == NULL || key == NULL){
        return -1;
    }
    return strlen(key) % table->size;
}

HashTable* HashTable_Init(unsigned int size){
    if(size < 1){
        printf("Error in HashTable initialization!\n");
        return NULL;
    }
    // Table inicialization
    HashTable *table = malloc(sizeof (HashTable));
    if(table == NULL){
        printf("Error in HashTable initialization!\n");
        return NULL;
    }
    // Buckets initialization
    table->buckets = calloc(size, sizeof (HashTableNode*));
    if(table->buckets == NULL){
        free(table);
        printf("Error in HashTable initialization!\n");
        return NULL;
    }
    // Table parameters
    table->count = 0;
    table->size = size;
    printf("HashTable successfully initialized.\n");
    return table;
}

bool HashTable_Insert(HashTable* table, Key key, Data* data){
    if(table == NULL || key == NULL || data == NULL){
        return false;
    }
    // Hash
    unsigned int index = Hash(table, key);

    //Duplicate key check
    HashTableNode* checkNode = table->buckets[index];
    while(checkNode != NULL){
        if(strcmp(checkNode->key, key) == 0){
            printf("Key already exists!\n");
            return false;
        }
        checkNode = checkNode->next;
    }

    if(table->count == table->size){
        printf("Rehashing table.\n");

        // new Buckets
        unsigned int newSize = table->size * 2;
        HashTableNode** newBuckets = calloc(newSize, sizeof (HashTableNode*));
        if(newBuckets == NULL){
            return false;
        }

        // Transfering nodes to new buckets
        HashTableNode* currentNode;
        for(unsigned int i = 0; i < table->size; i++){
            if(table->buckets[i] != NULL){
                currentNode = table->buckets[i];
                HashTableNode* nextNode;

                while(currentNode != NULL){
                    nextNode = currentNode->next;
                    currentNode->next = NULL;

                    // new Index
                    unsigned int newIndex = strlen(currentNode->key) % newSize;

                    // adding node to the new bucket
                    currentNode->next = newBuckets[newIndex];
                    newBuckets[newIndex] = currentNode;

                    currentNode = nextNode;
                }
            }
        }
        // removing old buckets
        free(table->buckets);

        // adding new buckets to the table and increasing size of the table
        table->buckets = newBuckets;
        table->size = newSize;
    }

    // Node creation
    HashTableNode* node = malloc(sizeof (HashTableNode));
    if(node == NULL){
        return false;
    }
    node->data = data;
    strcpy(node->key, key);
    node->next = table->buckets[index];

    // Adding node to the table
    table->buckets[index] = node;
    table->count++;
    printf("Node successfully created!\n");
    return true;
}

void ProcessFunction(unsigned int index, HashTableNode* node){
    if(node == NULL || index < 0){
        return;
    }
    unsigned int hash = strlen(node->key);
    printf("Hash: %d | ID: %d | Index: %d \n", hash, node->data->id, index);

}

void HashTable_Process(HashTable* table, NodeProc proc){
    if(table == NULL || proc == NULL){
        return;
    }
    for(unsigned int i = 0; i < table->size; i++){
        if(table->buckets[i] != NULL){
            HashTableNode* node = table->buckets[i];
            while(node != NULL){
                proc(i, node);
                node = node->next;
            }
        }
    }
}

void HashTable_Print(HashTable* table){
    if(table == NULL){
        return;
    }

    for(unsigned int i = 0; i < table->size; i++){
        if(table->buckets[i] != NULL){
            printf("Bucket: %d ||", i);
            HashTableNode* node = table->buckets[i];
            while(node != NULL){
                printf(" ID: %d",node->data->id);
                node = node->next;
                if(node != NULL){
                    printf(" |");
                }
            }
            printf("\n");
        }
    }
}

void ProcessFunction_PrintNodes();
