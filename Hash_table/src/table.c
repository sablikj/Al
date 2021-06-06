/**
 * @file       table.c
 * @author     Ondřej Ševčík
 * @date       6/2019
 * @brief      Implementation of functions for HashTable.
 * **************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

/* Private includes -------------------------------------------------------- */
#include "table.h"
#include <stdio.h>
#include <string.h>
#include "hash-private.h"
#include "mymalloc.h"

bool HashTable_Init(HashTable *table, size_t size, bool takeOwnership)
{
  if(table == NULL || size <= 0)
  {
      return false;
  }

  //Calloc
  //table->buckets = myCalloc(sizeof (HashTableNode*));
  //if (table->buckets  == NULL)
 // {
    //  return false;
  //}
  //Malloc
  table->buckets = myMalloc(size* sizeof (HashTableNode**));
  if (table->buckets  == NULL)
  {
      return false;
  }

  for(size_t i =0; i < size; i++)
  {
      table->buckets[i] = NULL;
  }
  table->count = 0;
  table->size = size;
  table->take_ownership= takeOwnership;

  return true;
}

void HashTable_Destruct(HashTable *table)
{
    //Destrukce tabulky, uvolnění všech indexů (buckets), nastavení velikosti na NULL;
    if(table == NULL)
    {
        return;
    }
    else
    {
        //Uvolnění paměti indexů (buckets)
        myFree(table->buckets);
        //Nastavení size a count na 0
        table->size = 0;
        //asi netřeba ten count or idk
        table->count = 0;
    }
}

bool HashTable_Insert(HashTable *table, Data_t *key, Data_t *value)
{
    if(table == NULL || key == NULL || value == NULL)
    {
        return false;
    }
    //Spočtáme hash do kterého bucketu budu ukládat
    size_t i = hash(table, key);
    HashTableNode* next = table->buckets[i];
    while(next != NULL)
    {
        //spočítáme jednotlivé položky těch bucketů, jestli se tam nenachází duplicitní klíč
        if(Data_Cmp(next->key, key) == 0)
        {
            //vložili bychom klíč, který již existuje, proto vracíme false
            return false;
        }
        next = next->next;
    }
    //Pokud není stejný tak můžu vytvořit nový spojový uzel a vložit na začátek spojového seznamu
    HashTableNode* node = myMalloc(sizeof (HashTableNode));
    if(node == NULL)
    {
        return false;
    }
    node->key = key;
    node->value = value;
    //Hodnota next ukazuje na ten aktuálně první prvek, takže na pozici buckets
    node->next = table->buckets[i];
    //buckets bude nově ukazovat ta první hodnota na ten první uzel
    table->buckets[i] = node;
    //zvýšíme počet uložených prvků
    table->count++;
    //vrátíme true
    return true;
}

bool HashTable_Replace(HashTable *table, Data_t *key, Data_t *value)
{
    //Replacneme item na keys pozici. Pokud takeOwnership je true, tak původní hodnota je "uvolněna". Pokud je hodnota replacnuta, tak vracíme true
    //Ošetření vstupů
    if(table == NULL || key == NULL || value == NULL || HashTable_Find(table,key) == NULL) //Pokud v tabulce nenajdeme prvek s hodnotou klíče, tak nemáme co replacovat, tím pádemvracíme false
    {
        return false;
    }
    //Spočítáme daný hash
    size_t hashed = hash(table, key);

    //Pokud by index (bucket) s daným hashem byl prázný (bez value atd.), vracíme false
    if(table->buckets[hashed] == NULL)
    {
        return false;
    }

    //Pokud bude take ownership true, tak původní hodnota je "uvolněna"
    if(table->take_ownership == true)
    {
        myFree(table->buckets[hashed]->value);
    }
    //Nahradíme value za nově zadanou value
    table->buckets[hashed]->value = value;
    //Po úspěšném provedení vracíme true
    return true;

}

bool HashTable_Delete(HashTable *table, Data_t *key)
{
    //Ošetření vstupů
    //Smažeme item, který je definován dle klíče. Pokud takový klíč není nic se nestane. Pokud je take ownership true, tak odstraníme data z itemu (Data_destruct)
    if(table == NULL || key == NULL)
    {
        return false;
    }

    HashTableNode *nalezenaNode = NULL;
    HashTableNode *predchoziNode = NULL;

    //Ověříme si, zda-li je tento klíč vůbec v tabulce
    if(HashTable_Find(table, key) == NULL)
    {
        return false;
    }
    //vytvoříme si node, která je = indexu (bucketu) s danným vypočítaným hashem
    HashTableNode *node = table->buckets[hash(table, key)];

    //Projdeme si všechny indexy, dokud nenajdeme key = key
    while(node != NULL)
    {
        //Klíče se shodují, node nalazena
        if(Data_Cmp(node->key, key) == 0)
        {
            nalezenaNode = node;
            break;
        }
        predchoziNode = node;
        //Procházíme všechyn node, dokud nenajdeme hledanou
        node = node->next;
    }

    //Pokud by byla nalezenaNode NULL, tak nemáme co mazat vrátíme false
    if(nalezenaNode == NULL)
    {
        return false;
    }

    //Pokud je take_ownership true, tak smažeme data v node pomocí Data_destruct
    if(table->take_ownership == true)
    {
        Data_Destruct(nalezenaNode->value);
        Data_Destruct(nalezenaNode->key);
    }
    //Přemístění nodů, pokud prechozíNode není null, tak pred.Node->next = nalezenaNode->next
    if(predchoziNode != NULL)
    {
        predchoziNode->next = nalezenaNode->next;
    }
    //Pokud by byla predchoziNode == NULL
    else
    {
        //index (bucket) dle hashe = nalezenaNode->next
        table->buckets[hash(table, key)] = nalezenaNode->next;
    }
    //Uvolnění paměti node, která má být odstraněna
    myFree(nalezenaNode);
    //Po odstranění zmenšíme počet prvků v hash tabulce
    table->count--;
    //Po úspěšném provedení vracíme true
    return true;
}

Data_t *HashTable_Find(HashTable *table, Data_t *key)
{
    //Vracíme data z itemu, které jsou identifikované klíčem (dle klíče najdu dané data)
    if(table == NULL || key == NULL)
    {
        return NULL;
    }
    //Spočítame hash
    size_t i = hash(table, key);
    //Vytvoříme si node, do které budeme ukládat všechny indexy (buckety), které budeme dále porovnávat
    HashTableNode *node = table->buckets[i];
    //Budeme proovnávat dokud node nebude NULL (žádné indexy)
    while(node != NULL)
    {
        //Poroivnáme klíč z tabulky a uživatelksý -> 0= match
        if(Data_Cmp(node->key, key) == 0)
        {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}

size_t HashTable_Get_Count(HashTable *table)
{
    //Ošetříme vstup
    if(table == NULL)
    {
        return 0;
    }
    //Vrátíme počet prvků
    else
    {
        return table->count;
    }
}

void HashTable_Clear(HashTable *table)
{
    //Vrací každou položku z tabulky, pokud je takeOwnership true, tak odstraníme data
    if (table == NULL)
    {
        return;
    }
    size_t i = 0;
    //Procházíme indexy do table_size
    while(i < table->size)
    {
        //Vytvoříme si node se kterou budeme pracovat, nastavíme její hodnotu na table->buckets[i];
        HashTableNode *nodeBucket = table->buckets[i];
        while(nodeBucket != NULL)
        {
            if(table->take_ownership == true)
            {
                //Odtsranění dat
                Data_Destruct(nodeBucket->key);
                Data_Destruct(nodeBucket->value);
            }
            HashTableNode *temp = nodeBucket;
            //node nastavíme aby smřovala na next node
            nodeBucket = nodeBucket->next;
            //Uvolníme temp node, která vlastní data z nodeBucket
            myFree(temp);
        }
        //Daný bucket (index) nastavíme na NULL
        table->buckets[i] = NULL;
        //Zmenšíme po uvolnění počet položek
        table->count--;
        i++;
    }
}

void HashTable_Process(HashTable *table, TableNodeProc proc)
{
    if(table == NULL || proc == NULL)
    {
        return;
    }
    //Proccesujeme každý item v table pomocí "proc" funkce
    HashTableNode* node;
    size_t i = 0;
    //Procházíme všechny prvky do velikosti tabulky
    while(i < table->size)
    {
        //do Node si ukládáme indexy(buckety)
        node=table->buckets[i];

        while(node != NULL)
        {
            proc(node->key, node->value);
            //Posuneme se na další node v "seznamu", dokud nebude NULL
            node = node->next;
        }
        i++;
    }
}
