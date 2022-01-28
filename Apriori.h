/*
 ------------------------------------------------------------------------------------------
|  This Apriori code for CS570 optimized with prefix hashing tree is my own work written   |
|  without consuting a tutor or code written by other students - Yuguang Meng, Sep 30 2015 |
 ------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include "node_Cn.h"
#include "node_data.h"
#include "TrieNode.h"

using namespace std;

void CreateTrie(TrieNode **, Node_tran *, int);
void Create_Cn1_tran(char *filename, Node_tran **tran, Node_Cn **Cn);
void CreateCn2more(TrieNode **, Node_Cn *, Node_Cn **, int, int);
void InsertTran2Trie(TrieNode **, Node_item *, int, int);
void combine(int *, int, int, int *, const int, char **, char **, TrieNode **);
void UpdateTran(Node_tran **, Node_Cn *);