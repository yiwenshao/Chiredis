#include"my_bench.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
benchRecord* loadFile(char* rfileName,char* wfileName,\
                                           unsigned long num){
    benchRecord* myload = (benchRecord*)malloc(sizeof(benchRecord));
    if(freopen(rfileName,"r",stdin)==NULL){
        puts("error freopen\n");
	return NULL;
    }
    myload->readFile=rfileName;
    myload->writeFile=wfileName;
    int i;
    myload->rkey = (char**)malloc(num*sizeof(char*));
    myload->rvalue = (char**)malloc(num*sizeof(char*));

    myload->wkey = (char**)malloc(num*sizeof(char*));
    myload->wvalue = (char**)malloc(num*sizeof(char*));
    
    myload->number=num;
    for(i=0;i<num;i++){
            
    }
}    

void closeBenchmark(benchRecord* br){
     if(br->readFile!=NULL)
         free(br->readFile);
     if(br->writeFile!=NULL)
         free(br->writeFile);

     if(br->rkey!=NULL){
         unsigned long i=0;
	 for(;i<br->number;i++){
	      if(br->rkey[i]!=NULL)
	         free(br->rkey[i]);
	 }
     }
     if(br->rvalue!=NULL){
         unsigned long i=0;
	 for(;i<br->number;i++){
	      if(br->rvalue[i]!=NULL)
	         free(br->rvalue[i]);
	 }
     }
     if(br->wvalue!=NULL){
         unsigned long i=0;
	 for(;i<br->number;i++){
	      if(br->wvalue[i]!=NULL)
	         free(br->wvalue[i]);
	 }
     }
     if(br->wkey!=NULL){
         unsigned long i=0;
	 for(;i<br->number;i++){
	      if(br->wkey[i]!=NULL)
	         free(br->wkey[i]);
	 }
     }
     free(br);
}

