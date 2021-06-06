#include "table.h"

// --------------- Function implementations -------------//

unsigned int Hash(HashTable *table, Key key){
    if(table == NULL || key == NULL){
        return false;
    }
    unsigned int h = strlen(key) % table->size;
    return h;
}

HashTable *HashTable_Init(unsigned int size){
    if(size < 1){
        return NULL;
    }

    // HashTable init
    HashTable *table = malloc(sizeof(HashTable));
    if(table == NULL){
        return NULL;
    }

    //Buckets init
    table->buckets = calloc(size, sizeof (HashTableNode*));
    if(table->buckets == NULL){
        free(table);
        return NULL;
    }

    // Setting table parameters
    table->size = size;
    table->count = 0;
    printf("HashTable initiated.\n");

    return table;
}

bool HashTable_Insert(HashTable *table, Key key, Data *data){
    if(table == NULL || key == NULL || data == NULL){
        return false;
    }

    // Creating hash
    unsigned int index = Hash(table, key);

    // Checking if key already exists in the table
    HashTableNode* checkNode = table->buckets[index];
    while(checkNode != NULL){
        if(strcmp(key, checkNode->key) == 0){
            printf("Key already exists!\n");
            return false;
        }
        checkNode = checkNode->next;
    }

    // Creating new node with data
    HashTableNode* node = malloc(sizeof(HashTableNode));
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

    // Checking if rehash is needed
    if(table->count == table->size){
        printf("Rehashing table.\n");

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

                    // New index for new buckets
                    unsigned int newIndex = strlen(currentNode->key) % newSize;

                    // Adding node to new buckets
                    currentNode->next = newBuckets[newIndex];
                    newBuckets[newIndex] = currentNode;

                    // Moving to next node
                    currentNode = nextNode;
                }
            }
        }

        // Freeing old buckets
        free(table->buckets);

        // Adding new buckets to the table and settig new size
        table->buckets = newBuckets;
        table->size = newSize;
    }
    return true;
}

void ProcessFunction(Key key, HashTableNode *node){
    if(key == NULL || node == NULL){
        return;
    }
    unsigned int hash = strlen(node->key);
    printf("<PF> Hash: %u | ID: %u | Index: %s \n", hash, node->data->id, key);
}

void HashTable_Process(HashTable *table, NodeProc proc){
    if(table == NULL || proc == NULL){
        return;
    }

    for(unsigned int i = 0; i < table->size; i++){
        HashTableNode* currentNode = table->buckets[i];

        while(currentNode != NULL){
            proc(i, currentNode);
            printf("<TF Hash: %u | ID: %u | Index: %u \n", (unsigned)strlen(currentNode->key), currentNode->data->id, i);
        }
    }
}

void HashTable_Print(HashTable *table){
    if(table == NULL){
        return;
    }

    for(unsigned int i = 0; i < table->size; i++){
        printf("Bucket <index: %u> ||", i);
        if(table->buckets[i] != NULL){
            HashTableNode* currentNode = table->buckets[i];

            while(currentNode != NULL){
                printf("<ID: %u", currentNode->data->id);
                currentNode = currentNode->next;
                if(currentNode != NULL){
                    printf(" |");
                }
            }
        }
        printf("\n");
    }
}
