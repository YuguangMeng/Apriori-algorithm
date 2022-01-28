/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node_Cn
{
  char *element[32];
  int n;  //element number
  int count;
  Node_Cn *prev;
  Node_Cn *next;
} Node_Cn;

void InitCn(Node_Cn **);
int InsertCn(Node_Cn **, char **, int, int);
void PruneCn(Node_Cn **, int);
int ReadCn(Node_Cn *);
void ClearCn(Node_Cn **);
int StringCompare(Node_Cn *, char **, int);
int IsSubCnInCn_1(Node_Cn *, char **, int);
void write_Cn(Node_Cn **, char **, int, int, FILE *);