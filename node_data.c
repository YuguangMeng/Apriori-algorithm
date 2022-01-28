/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include "node_data.h"

void InitItem(Node_item **pNode)
{
  *pNode=NULL;
}

void InitTran(Node_tran **pNode)
{
  *pNode=NULL;
}

int InsertItem(Node_item **pNode, char *name)
{
  Node_item *pInsert;
  Node_item *pHead, *pHead_prev=NULL;

  pHead = *pNode;

  if (!(pInsert = (Node_item *)malloc(sizeof(Node_item))))
    return 0;
  strcpy(pInsert->name, name);
  pInsert->next=NULL;

  if(!pHead)
  {
    pHead = pInsert;
    *pNode=pHead;
  }
  else
  {
    while( pHead->next &&
           (strlen(pHead->name)<strlen(pInsert->name) ||
            (strlen(pHead->name)==strlen(pInsert->name) && strcmp(pHead->name, pInsert->name)<0))
         )
    {
      pHead_prev=pHead;
      pHead = pHead->next;
    }
    if(  (strlen(pHead->name)>strlen(pInsert->name) ||
         (strlen(pHead->name)==strlen(pInsert->name) && strcmp(pHead->name, pInsert->name)>=0))
      )
    {
      if(pHead_prev)
        pHead_prev->next=pInsert;
      else
        *pNode=pInsert;
      pInsert->next=pHead;
    }
    else
      pHead->next=pInsert;
  }
  return 1;
}

int InsertTran(Node_tran **pNode, char *s)
{
  Node_tran *pInsert;
  Node_tran *pHead=*pNode;
  Node_item *item;

  char *p, *str, buf[256];
  const char *d = " \t\n";

  if (!(pInsert = (Node_tran *)malloc(sizeof(Node_tran))))
    return 0;
  pInsert->next=NULL;
  pInsert->nItem=0;

  InitItem(&item);
  strcpy(buf,s);
  p = strtok(buf,d);
  while(p)
  {
    str=(char *)malloc(strlen(p)+1);
    RemoveSpace(str, p);
    if(strlen(str)!=0)
      if(InsertItem(&item, str))
        pInsert->nItem++;

    free(str);
    p=strtok(NULL,d);
  }
  pInsert->item=item;

  if(!pHead)
  {
    pHead = pInsert;
    *pNode=pHead;
  }
  else
  {
    *pNode=pInsert;
    pInsert->next=pHead;
  }
  return 1;
}

void ClearTran(Node_tran **tran)
{
  Node_tran *pTran, *pTranNext=*tran;
  while(pTranNext)
  {
    pTran=pTranNext;
    ClearOneTran(pTran);
    pTranNext=pTranNext->next;
  }
}

void ClearOneTran(Node_tran *tran)
{
  Node_item *pItem, *pItemNext;
  pItemNext=tran->item;
  while(pItemNext)
  {
    pItem=pItemNext;
    pItemNext=pItemNext->next;
    free(pItem);
  }
}

void ReadTran(Node_tran *pHead)
{
  Node_item *item=NULL;
  while(pHead)
  {
    item=pHead->item;
    while(item)
    {
      printf("%s ", item->name);
      item=item->next;
    }
    printf("\n");
    pHead=pHead->next;
  }
}

void RemoveSpace(char *s, char *p)
{
  char *p1=p;
  while(*p1!='\0')
  {
    if(IsLetter(*p1) || IsDigit(*p1))
    {
      *s=*p1;
      s++;
    }
    p1++;
  }
  *s='\0';
}

int IsLetter(char c) //Letter
{
  if(((c<='z')&&(c>='a'))||((c<='Z')&&(c>='A')))
    return 1;
  else
    return 0;
}

int IsDigit(char c) //Digit
{
  if(c>='0'&&c<='9')
    return 1;
  else
    return 0;
}