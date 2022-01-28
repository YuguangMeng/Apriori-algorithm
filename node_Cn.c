/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include "node_Cn.h"

void InitCn(Node_Cn **pNode)
{
  *pNode=NULL;
}

int InsertCn(Node_Cn **pNode, char **insertElem, int n, int count)
{
  int i;
  Node_Cn *pInsert;
  Node_Cn *pHead;

  pHead = *pNode;

  if (!(pInsert = (Node_Cn *)malloc(sizeof(Node_Cn))))
    return 1;

  for(i=0; i<n; i++)
  {
    pInsert->element[i] = (char *)malloc(strlen(insertElem[i])+1);
    strcpy(pInsert->element[i], insertElem[i]);
  }
  pInsert->n=n;
  pInsert->count=count;
  pInsert->prev=pInsert->next=NULL;

  if(!pHead)
  {
    pHead = pInsert;
    *pNode=pHead;
  }
  else
  {
    while(StringCompare(pHead, pInsert->element, pHead->n)<0 && pHead->next)
      pHead = pHead->next;
      
    if(StringCompare(pHead, pInsert->element, pHead->n)==0)
    {
      (pHead->count)+=count;
      free(pInsert);
    }      
    else if(StringCompare(pHead, pInsert->element, pHead->n)<0 && !pHead->next)
    {
      pInsert->prev=pHead;
      pInsert->next=pHead->next;
      pHead->next=pInsert;
    }
    else //if(StringCompare(pHead, pInsert->element, pHead->n)>0)
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

int ReadCn(Node_Cn *pHead)
{
  int i, n, size = 0;
  while(pHead != NULL)
  {
    size++;
    n=pHead->n;
    for(i=0; i<n; i++)
      printf("%s ", pHead->element[i]);
    printf("(%d)\n",pHead->count);
    pHead = pHead->next;
  }
  printf("The number of low in C%d is (%d)\n", n, size);
}

void PruneCn(Node_Cn **pNode, int sup)
{
  Node_Cn *pHead=*pNode;
  Node_Cn *pHead2=NULL;
  int i=0;
  while(pHead)
  {
  	i++;
    if(pHead->count<sup)
    {
    	i--;
      pHead2=pHead;
      if(!pHead->prev)
      {
        if(pHead->next)
          pHead->next->prev=pHead->prev;
        *pNode=pHead->next;
      }
      else
      {
        pHead->prev->next=pHead2->next;
        if(pHead2->next)
          pHead2->next->prev=pHead2->prev;
      }
      pHead=pHead->next;
      free(pHead2);
    }
    else
      pHead=pHead->next;
  }
}

void ClearCn(Node_Cn **Cn)
{
  int i;
  Node_Cn *pNext=*Cn, *pHead;

  while(pNext)
  {
    pHead=pNext;
    pNext=pNext->next;
    for(i=0; i<pHead->n; i++)
      free(pHead->element[i]);
    free(pHead);
  }
}

int StringCompare(Node_Cn *pHead, char **element, int n) //<: -1;  =: 0;  > 1
{
  int i;
  for(i=0; i<n; i++)
    if((strcmp(pHead->element[i], element[i])<0 && strlen(pHead->element[i])==strlen(element[i])) ||
        strlen(pHead->element[i])<strlen(element[i]))
      return -1;
    else if((strcmp(pHead->element[i], element[i])>0 && strlen(pHead->element[i])==strlen(element[i])) ||
            strlen(pHead->element[i])>strlen(element[i]))
      return 1;
  return 0;
}

int IsSubCnInCn_1(Node_Cn *Cn_1, char **Cn_item, int n)
{
  char *sub_Cn_item[32];
  int i, j, f, k, is;
  Node_Cn *Cn_1_Head=Cn_1;

  if(n==2)
    return 1;

  for(k=0; k<n-2; k++)
  {
    is=0;
    f=0;
    for(i=0; i<n-1; i++)
    {
      if(i==k)
        f=1;
      if(f)
        j=i+1;
      else
        j=i;

      sub_Cn_item[i] = (char *)malloc(strlen(Cn_item[j])+1);
      strcpy(sub_Cn_item[i], Cn_item[j]);
    }

    Cn_1=Cn_1_Head;
    while(Cn_1 && StringCompare(Cn_1, sub_Cn_item, Cn_1->n)<=0)
    {
      if(StringCompare(Cn_1, sub_Cn_item, Cn_1->n)==0)
      {
        is=1;
        break;
      }
      Cn_1=Cn_1->next;
    }
    if(!is)
      return 0;
      
    for(i=0; i<n-1; i++)
        free(sub_Cn_item[i]);
  }
  return is;
}

void write_Cn(Node_Cn **Cn, char **buf, int n, int nMax, FILE *fp)
{
  int i;
  Node_Cn *p;

  p=Cn[n];
  if(n<nMax)
    while(p && (n<1?1:StringCompare(p, buf, n)<=0))
    {
      if(n<1?1:(StringCompare(p, buf, n)==0))
      {
        buf[n]=(char *)malloc(strlen(p->element[n])+1);
        strcpy(buf[n], p->element[n]);
        for(i=0; i<p->n; i++)
          fprintf(fp, "%s ", buf[i]);
        fprintf(fp,"(%d) \n", p->count);
        write_Cn(Cn, buf, n+1, nMax, fp);
      }
      p=p->next;
    }
}