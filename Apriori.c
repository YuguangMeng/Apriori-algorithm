/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include "Apriori.h"

int main(int argc, char ** argv)
{
  int i=0, sup, nMax;
  TrieNode *trie;
  Node_tran *tran;
  Node_Cn *Cn[32];
  char *buf[32];
  FILE *fp;
  clock_t start, finish;
  double Prev_Time, Now_Time, totaltime;

  if(argc!=2)
  {
    printf("Usage: %s filename!\n", argv[0]);
    exit(0);
  }

  printf("\n");
  printf(" -----------------------------------------------------------------------------------------\n");
  printf("|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written  |\n");
  printf("|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015|\n");
  printf(" -----------------------------------------------------------------------------------------\n");
  printf("\n");

  do
  {
    printf("The data file is: %s. ", argv[1]);
    printf("Please enter the minimum support more than 0: ");
    scanf ("%d", &sup);
  } while(sup<1);
  printf("\n");

  start=clock();
  InitCn(&Cn[0]);
  InitTran(&tran);

  Create_Cn1_tran(argv[1], &tran, Cn);
  PruneCn(&Cn[0], sup);

  Now_Time=clock();
  printf("The running time for C1 calculation is %.1f s!\n",(Now_Time-start)/CLOCKS_PER_SEC);

  while(Cn[i])
  {
    nMax=i+1;
    i++;
    if(nMax>1)
      UpdateTran(&tran, Cn[nMax-1]);

    InitCn(&Cn[i]);
    InitTrieRoot(&trie);
    CreateTrie(&trie, tran, i+1);

    Prev_Time=Now_Time;
    Now_Time=clock();
    printf("The running time for prefix hashing tree calculation for C%d is %.1f s!\n", i+1, (Now_Time-Prev_Time)/CLOCKS_PER_SEC);

    CreateCn2more(&trie, Cn[i-1], &Cn[i], i+1, sup);
    if(Cn[i])
    {
      Prev_Time=Now_Time;
      Now_Time=clock();
      printf("The running time for C%d calculation is %.1f s!\n", i+1, (Now_Time-Prev_Time)/CLOCKS_PER_SEC);
    }
    ClearTrie(&trie);
  }

  finish=clock();
  totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
  printf("\n");
  printf("The TOTAL running time is ONLY %.1f s!\n", totaltime);
  printf("\n");
  printf("The frequency itemsets have been written to out.txt in the same directory!\n");

  if(fp=fopen("out.txt","w"))
  {
    write_Cn(Cn, buf, 0, nMax, fp);
    fclose(fp);
  }

  for(i=0; i<nMax; i++)
    ClearCn(&Cn[i]);

  ClearTran(&tran);

  return 0;
}

void Create_Cn1_tran(char *filename, Node_tran **tran, Node_Cn **Cn)
{
  FILE *fp;
  char buf[256];
  const char *d = " \t\n";
  char *p, *s;

  if((fp=fopen(filename,"r"))==NULL)
  {
    printf("No such file!\n");
    exit(0);
  }

  while(fgets (buf, 256, fp)!=NULL)
  {
    InsertTran(tran, buf);
    p = strtok(buf,d);
    while(p)
    {
      s=(char *)malloc(strlen(p)+1);
      RemoveSpace(s, p);
      if(strlen(s)!=0)
        InsertCn(Cn, &s, 1, 1);
      free(s);
      p=strtok(NULL,d);
    }
  }

  fclose(fp);
}

void CreateTrie(TrieNode **trie, Node_tran *tran, int n_C)
{
  while(tran)
  {
    if(tran->item)
      InsertTran2Trie(trie, tran->item, tran->nItem, n_C);
    tran=tran->next;
  }
}

void CreateCn2more(TrieNode **trie, Node_Cn *Cn_1, Node_Cn **Cn, int n, int sup)
{
  int i, count;
  char *buf[32];
  Node_Cn *Cn_1_f=NULL;

  while(Cn_1 != NULL)
  {
    for(i=0; i<n-2; i++)
    {
      buf[i] = (char *)malloc(strlen(Cn_1->element[i])+1);
      strcpy(buf[i], Cn_1->element[i]);
    }

    Cn_1_f=Cn_1->next;

    while(n==2 && Cn_1_f)
    {
      buf[0] = (char *)malloc(strlen(Cn_1->element[0])+1);
      strcpy(buf[0], Cn_1->element[0]);
      buf[1] = (char *)malloc(strlen(Cn_1_f->element[0])+1);
      strcpy(buf[1], Cn_1_f->element[0]);
      count=SearchTrie(trie, buf, n, sup);
      if(count>=sup)
        InsertCn(Cn, buf, n, count);
      free(buf[0]);
      free(buf[1]);
      Cn_1_f=Cn_1_f->next;
    }

    while(n>=3 && Cn_1_f && StringCompare(Cn_1_f, buf, n-2)<=0)
    {
      if(StringCompare(Cn_1_f, buf, n-2)==0)
      {
        buf[n-2] = (char *)malloc(strlen(Cn_1->element[n-2])+1);
        strcpy(buf[n-2], Cn_1->element[n-2]);

        buf[n-1] = (char *)malloc(strlen(Cn_1_f->element[n-2])+1);
        strcpy(buf[n-1], Cn_1_f->element[n-2]);

        if(IsSubCnInCn_1(Cn_1, buf, n))
        {
          count=SearchTrie(trie, buf, n, sup);
          if(count>=sup)
            InsertCn(Cn, buf, n, count);
        }

        free(buf[n-2]);
        free(buf[n-1]);
      }
      Cn_1_f=Cn_1_f->next;
    }

    for(i=0; i<n-2; i++)
      free(buf[i]);

    Cn_1 = Cn_1->next;
  }
}

void InsertTran2Trie(TrieNode **trie, Node_item *item, int n, int n_C)
{
  int *a=NULL, *b=NULL;
  int i=0;
  char *s[32];
  char *subs[32];

  a=(int *)malloc(sizeof(int)*n);
  for(i=0; i<n; i++)
    a[i]=i+1;

  i=0;
  while(item)
  {
    s[i]=(char *)malloc(1+strlen(item->name));
    strcpy(s[i], item->name);
    i++;
    item=item->next;
  }

  if(n>=n_C)
  {
    b=(int *)malloc(sizeof(int)*n_C);
    combine(a, n, n_C, b, n_C, s, subs, trie);
    free(b);
  }

  free(a);

  for(i=0; i<n; i++)
    free(s[i]);

}

void combine(int a[], int n, int m,  int b[], const int M, char **s, char **subs, TrieNode **trie)
{
  int k;
  for(int i=n; i>=m; i--)
  {
    b[m-1] = i - 1;
    if (m > 1)
      combine(a,i-1,m-1,b,M, s, subs, trie);
    else
    {
      k=0;
      for(int j=0; j<=M-1; j++)
      {
        subs[k]=(char *)malloc(1+strlen(s[a[b[j]]-1]));
        strcpy(subs[k], s[a[b[j]]-1]);
        k++;
      }

      InsertTrie(trie, subs, M);

      for(k=0; k<M; k++)
        free(subs[k]);
    }
  }
}

void UpdateTran(Node_tran **pNode, Node_Cn *Cn)
{
  Node_tran *pHead=*pNode, *pHead_prev=NULL;
  Node_item *item_prev=NULL, *itemHead=NULL, *itemHead2=NULL;
  Node_Cn *CnHead=Cn;
  int i, match=0;

  while(pHead)
  {
    itemHead=pHead->item;

    item_prev=NULL;
    itemHead2=NULL;
    while(itemHead)
    {
      match=0;
      CnHead=Cn;
      while(CnHead &&
            (strlen(itemHead->name)>strlen(CnHead->element[0]) ||
             (strlen(itemHead->name)==strlen(CnHead->element[0]) && strcmp(itemHead->name, CnHead->element[0])>=0)))
      {
        for(i=0; i<CnHead->n; i++)
          if(strcmp(CnHead->element[i], itemHead->name)==0)
          {
            match=1;
            break;
          }
        if(match)
          break;
        CnHead=CnHead->next;
      }

      if(!match)
      {
        pHead->nItem--;
        itemHead2=itemHead;
        if(item_prev)
          item_prev->next=itemHead->next;
        else
          pHead->item=itemHead->next;
        itemHead=itemHead->next;
        free(itemHead2);
      }
      else
      {
        item_prev=itemHead;
        itemHead=itemHead->next;
      }
    }
    //when one transaction is empty, delete it!

    if(!pHead->item)
    {
      if(!pHead_prev)
      {
        *pNode=pHead->next;
        free(pHead);
        pHead_prev=NULL;
        pHead=*pNode;
      }
      else
      {
        pHead_prev->next=pHead->next;
        free(pHead);
        pHead=pHead_prev->next;
      }
    }
    else
    {
      pHead_prev=pHead;
      pHead=pHead->next;
    }
  }
}