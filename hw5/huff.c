#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string.h>
#include "huffman.h"


int main(int argc, char** argv){
  int *warray=0,*wArr=0;
  char*carray=0,*cArr=0;
  char temp;
  int size=0,i=0,j=0,f=0;
  minheap* mhp;
  tnode* huff_tree;

  FILE *fptr =fopen(argv[1],"r");
  
  
  if (fptr == NULL){
    fprintf
      (stderr,"Cannot load file %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  
  int length=strlen(argv[1]);
  char *outfile_name=(char*)malloc((length+5)*sizeof(char));
  sprintf(outfile_name,"%s.huff",argv[1]);
  FILE *outfptr=fopen(outfile_name,"wb");
    
  warray = (int*)malloc(256*sizeof(int));
  carray = (char*)malloc(256*sizeof(char));
  wArr = (int*)malloc(256*sizeof(int));
  cArr = (char*)malloc(256*sizeof(int));
  while((temp = fgetc(fptr)) != EOF){ 
    wArr[(int)temp]+=1;
    if(wArr[(int)temp]==1){
      cArr[(int)temp]=temp;
      size+=1;
      }
  }
  for(i=0;i<256;i++){
   
      if(wArr[i]){
	warray[j]=wArr[i];
	carray[j]=cArr[i];
	j++;
      }

    }

  mhp = construct_minheap(carray,warray,size);
  mhp->array=(tnode**)realloc(mhp->array,(mhp->size+1)*sizeof(tnode*));
  insert_tnode(mhp,construct_tnode(EOF,1));
  size++;
  huff_tree = huffman_tree(mhp);

  int cnt=0;
  char input;
  unsigned char bit_fld=0;
  int *sequence =(int*)malloc(30*sizeof(int));
  i=0;
  *sequence=0;
  rewind(fptr);
  fwrite(&size,4,1,outfptr);
  print_tree(huff_tree,outfptr,&cnt,&bit_fld);
  while((input=fgetc(fptr))!=EOF){
    i=0;
    f=0;
    find_sequence(huff_tree,i,input,sequence,&bit_fld,&cnt,outfptr,&f);
    
  }  
  i=0;
  f=0;
  find_sequence(huff_tree,i,EOF,sequence,&bit_fld,&cnt,outfptr,&f);
  fwrite(&bit_fld,1,1,outfptr);
  free(warray);
  free(carray);
  free(wArr);
  free(cArr);
  free(sequence);
  free(mhp);
  free(huff_tree);
  fclose(fptr);
  fclose(outfptr);
 
  return 0;

  

}

tnode *construct_tnode(char Data,int Weight){
  tnode* new = (tnode *)malloc(sizeof(tnode));
  new -> data = Data;
  new -> weight = Weight;
  new -> left = NULL;
  new -> right = NULL;
  return new;
}

minheap *init_minheap(int Size){
  minheap* mhp=(minheap*)malloc(sizeof(minheap));
  mhp->size =Size;
  mhp->array=(tnode**)malloc(Size*sizeof(tnode*));
  return mhp;
}


void insert_tnode(minheap* mhp,tnode* new){
  mhp->size+=1;
  int index=mhp->size-1;
  while (index && new->weight < mhp->array[(index-1)/2]->weight){
    mhp->array[index] = mhp->array[(index-1)/2];
    index =(index-1)/2;
  }
  mhp->array[index]=new;
}

void swap_tnode(tnode** a,tnode** b){
  tnode* temp = *a;
  *a = *b;
  *b = temp;
}
  

void heapify(minheap* mhp,int parent){
  int min = parent;
  int left =2*parent+1;
  int right = 2*parent+2;
  if(left<mhp->size && (mhp->array[left]->weight) < (mhp->array[min]->weight)){
    min = left;
  }
  if(right<mhp->size && (mhp->array[right]->weight) < (mhp->array[min]->weight)){
    min = right;
  }
  if(parent != min){
    swap_tnode(&mhp->array[min],&mhp->array[parent]);
    heapify(mhp,min);
  }
  
}

tnode* delete_tnode(minheap*mhp){
  tnode* temp = mhp->array[0];
  mhp->array[0]=mhp->array[(mhp->size)-1];
  --mhp->size;
  heapify(mhp,0);
  return temp;
}

minheap* construct_minheap(char *carray,int *warray,int Size){
  minheap* mhp = init_minheap(Size);
  int i;
  for(i=0;i<Size;i++){
    mhp->array[i]=construct_tnode(carray[i],warray[i]);
  }
  mhp->size=Size;
   for(i=(mhp->size-2)/2;i>=0;--i){
    heapify(mhp,i);
    }
  return mhp;
}

tnode* huffman_tree(minheap*mhp){
  tnode* left;
  tnode* right;
  tnode* root;
  
  while(mhp->size>1){
    left = delete_tnode(mhp);
    right = delete_tnode(mhp);
    root = construct_tnode('@',left->weight+right->weight);
    root->left=left;
    root->right=right;
    insert_tnode(mhp,root);
  }
  return delete_tnode(mhp);
}

void InorderPrint(tnode*node){
  if(node == NULL){
    return;
  }
  InorderPrint(node->left);
  printf("%d\n",node->weight);
  InorderPrint(node->right);
}

void find_sequence(tnode*root,int i,char key,int*sequence,unsigned char *bit_fld,int*cnt,FILE*fptr,int*f){
  if(*f==1){return;}
  if(root->left){
    sequence[i]=0;
    find_sequence(root->left,i+1,key,sequence,bit_fld,cnt,fptr,f);
  }
  if(root->right){
    sequence[i]=1;
    find_sequence(root->right,i+1,key,sequence,bit_fld,cnt,fptr,f);
  }
      
  if((root->left==NULL)&&(root->right==NULL)){
    if(root->data == key){
      int j;
      *f=1;
      for(j=0;j<i;j++){
	if(sequence[j]==1){
	  write_bits(bit_fld,1,cnt,fptr);
	}
	if(sequence[j]==0){
	  write_bits(bit_fld,0,cnt,fptr);
	}
      }
    }     
      
  }
}


void print_tree(tnode*root,FILE*fptr,int*cnt,unsigned char* bit_fld){
  if((root->left==NULL) && (root->right==NULL)){
    write_bits(bit_fld,1,cnt,fptr);
    //printf("%d",1);
    write_bits(bit_fld,(unsigned char)(root->data),cnt,fptr);
    //printf("%c",root->data); 
    return;
  }
  write_bits(bit_fld,0,cnt,fptr);
  //printf("%d",0);
  print_tree(root->left,fptr,cnt,bit_fld);
  print_tree(root->right,fptr,cnt,bit_fld);
  return;
}
  

 void write_bits(unsigned char *bit_fld,unsigned char num,int*cnt,FILE*fptr){
   unsigned char buff=0; 

   if(*cnt == 8){
     
       fwrite(bit_fld,1,1,fptr);
       *bit_fld=0;
       *cnt=0;
     }
     if(num == 0){
       *cnt+=1;
     }
     else if(num == 1){
       *bit_fld |= (1<<(7-*cnt));
       *cnt+=1;
     }
     else{
       buff = num;
       buff>>=*cnt;
       *bit_fld+=buff;
       fwrite(bit_fld,1,1,fptr);
       *bit_fld=0;
       buff = num;
       buff<<=(8-*cnt);
       *bit_fld+=buff;
     }
       
       
     
	
   
}


