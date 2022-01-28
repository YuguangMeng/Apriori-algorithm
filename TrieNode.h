/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_Items 32
#define MAX_Children 100

typedef struct ItemList
{
    char *name[MAX_Items];
    int n;
    int nCount;
    ItemList *prev;
    ItemList *next;
} ItemList;

typedef struct TrieNode
{
    ItemList *pItemList;
    int nCount;
    TrieNode* next[MAX_Children];
} TrieNode;


void InitTrieRoot(TrieNode **);
TrieNode* CreateTrieNode();
void InsertTrie(TrieNode **, char **, int);
int SearchTrie(TrieNode **, char **, int, int);
void ClearTrie(TrieNode **);
int InsertItemList(ItemList **, char **, int);
int ItemCompare(ItemList *, char **, int);