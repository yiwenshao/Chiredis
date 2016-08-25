#include"my_bench.h"
#include"connect.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
*input: add any valid filename
*
*/
void pipe_example(char* filename){
     int port=6379;
     char* ip="127.0.0.1";
     FILE* fp;
     singleClient* sc = single_connect(port,ip);
     if(fp=freopen(filename,"a",stdin)==NULL){
         fprintf(stderr,"error redirection\n");
     }
     char* key=(char*)malloc(500);
     char* value=(char*)malloc(500);
     char* revalue=(char*)malloc(500);

     pipe_set(sc,"testk","testv");
     pipe_set(sc,"testk1","testv1");
     pipe_get(sc,"testk");
     pipe_get(sc,"testk1");

     pipe_getReply(sc,revalue);
     puts(revalue);
     pipe_getReply(sc,revalue);
     puts(revalue);
     pipe_getReply(sc,revalue);
     puts(revalue);
     pipe_getReply(sc,revalue);
     puts(revalue);
}

