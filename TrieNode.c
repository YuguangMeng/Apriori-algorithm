/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include "TrieNode.h"

void InitTrieRoot(TrieNode **pRoot)
{
  *pRoot=NULL;
}

TrieNode* CreateTrieNode()
{
  int i;
  TrieNode *p;

  p=(TrieNode *)malloc(sizeof(TrieNode));
  p->pItemList=NULL;
  p->nCount=0;
  for(i=0; i<MAX_Children; i++)
    p->next[i]=NULL;
  return p;
}

void InsertTrie(TrieNode **pRoot, char **s, int len)
{
  int i,k;
  TrieNode*p;
  if(!(p=*pRoot))
  {
    p=*pRoot=CreateTrieNode();
  }
  for(i=0; i<len; i++)
  {
    k=atoi(s[i]) % MAX_Children;
    if(!p->next[k])
      p->next[k]=CreateTrieNode();
    p=p->next[k];
  }

  InsertItemList(&(p->pItemList), s, len);
  p->nCount++;
}

int SearchTrie(TrieNode **pRoot,char **s, int len, int sup)
{
  TrieNode *p;
  ItemList *pItem;
  int i,k;
  if(!(p=*pRoot))
  {
    return 0;
  }
  i=0;
  for(i=0; i<len; i++)
  {
    k=atoi(s[i]) % MAX_Children;
    if(p->next[k]==NULL) return 0;
    p=p->next[k];
  }

  if(p->nCount<sup)
    return 0;

  pItem=p->pItemList;
  while(pItem->next && ItemCompare(pItem, s, len)<0)
    pItem=pItem->next;

  if(ItemCompare(pItem, s, len)==0)
    return pItem->nCount;
  else
    return 0;
}


int InsertItemList(ItemList **pNode, char **insertElem, int n)
{
  int i;
  ItemList *pInsert;
  ItemList *pHead;

  pHead = *pNode;

  if (!(pInsert = (ItemList *)malloc(sizeof(ItemList))))
    return 1;

  for(i=0; i<n; i++)
  {
    pInsert->name[i] = (char *)malloc(strlen(insertElem[i])+1);
    strcpy(pInsert->name[i], insertElem[i]);
  }
  pInsert->n=n;
  pInsert->nCount=1;
  pInsert->prev=pInsert->next=NULL;

  if(!pHead)
  {
    pHead = pInsert;
    *pNode=pHead;
  }
  else
  {
    while(ItemCompare(pHead, pInsert->name, pHead->n)<0 && pHead->next)
      pHead = pHead->next;

    if(ItemCompare(pHead, pInsert->name, pHead->n)==0)
    {
      pHead->nCount++;
      free(pInsert);
    }
    else if(ItemCompare(pHead, pInsert->name, pHead->n)<0 && !pHead->next)
    {
      pInsert->prev=pHead;
      pInsert->next=pHead->next;
      pHead->next=pInsert;
    }
    else //if(ItemCompare(pHead, pInsert->name, pHead->n)>0)
    {
      pInsert->prev=pHead->prev;
      pInsert->next=pHead;
      if(!pHead->prev)
        *pNode=pInsert;
      else
        pHead->prev->next=pInsert;
      pHead->prev=pInsert;
    }
  }

  return 0;
}

void ClearTrie(TrieNode **root_trie)
{
  ItemList *pItem_next, *pName_next, *pItem;
  TrieNode *trie=*root_trie;
  if(trie)
  {
    pItem_next=trie->pItemList;
    while(pItem_next)
    {
      pItem=pItem_next;
      pItem_next=pItem_next->next;
      for(int i=0; i<pItem->n; i++)
        if(pItem->name[i])
          free(pItem->name[i]);
      free(pItem);
    }
    for(int j=0; j<MAX_Children; j++)
    if(trie->next[j])
      ClearTrie(&trie->next[j]);
    free(trie);
  }
}

int ItemCompare(ItemList *pHead, char **name, int n) //<: -1;  =: 0;  > 1
{
  int i;
  for(i=0; i<n; i++)
    if((strcmp(pHead->name[i], name[i])<0 && strlen(pHead->name[i])==strlen(name[i])) ||
        strlen(pHead->name[i])<strlen(name[i]))
      return -1;
    else if((strcmp(pHead->name[i], name[i])>0 && strlen(pHead->name[i])==strlen(name[i])) ||
            strlen(pHead->name[i])>strlen(name[i]))
      return 1;
  return 0;
}