#ifndef __huffman_h_
#define __huffman_h_

#include <stdlib.h>
#include <stdio.h>
#define MASK 0x80
typedef struct _tnode{
  int weight;
  char data; 
  struct _tnode*left;
  struct _tnode*right;
}tnode;

typedef struct minheap{
  int size;
  tnode **array;
}minheap;
  
tnode *construct_tnode(char Data,int Weight);
minheap *init_minheap(int Size);
void insert_tnode(minheap* mhp,tnode* new);
void swap_tnode(tnode** a,tnode** b);
void heapify(minheap* mhp,int parent);
tnode* delete_tnode(minheap*mhp);
minheap* construct_minheap(char *carray,int *warray,int Size);
tnode* huffman_tree(minheap*mhp);
void InorderPrint(tnode*node);
void write_bits(unsigned char *bit_fld,unsigned char num,int*cnt,FILE*fptr);
void print_tree(tnode*root,FILE*fptr,int*cnt,unsigned char* bit_fld);
void find_sequence(tnode*root,int i,char key,int*sequence,unsigned char *bit_fld,int*cnt,FILE*fptr,int*f);
void read_tree(FILE *fptr,tnode*root,int size,int*ccnt,unsigned char*bit_fld,int*cnt);
unsigned char read_bits(unsigned char*bit_fld,int num,int*cnt,FILE*fptr);
void read_sequence(tnode*root,int d,unsigned char*bit_fld,int*cnt,FILE*fptr);

#endif
