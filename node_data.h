/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node_item
{
  char name[8];
  Node_item *next;
} Node_item;

typedef struct Node_tran
{
  Node_item *item;
  int  nItem;
  Node_tran *next;
} Node_tran;

void InitItem(Node_item **);
void InitTran(Node_tran **);
int InsertItem(Node_item **, char *);
int InsertTran(Node_tran **, char *);
void ReadTran(Node_tran *);
void ClearTran(Node_tran **);
void ClearOneTran(Node_tran *);
void RemoveSpace(char *, char *);
int IsLetter(char);
int IsDigit(char);