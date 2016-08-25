#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>
#include "my_bench.h"
/*
void single_pipe_bench_set(char* filename,int num){
     int port=6379;
     char* ip="127.0.0.1";
     FILE* fp;
     singleClient* sc = single_connect(port,ip); 

     if(fp=freopen(filename,"r",stdin)==NULL){
         fprintf(stderr,"error redirection\n");
     }
     char* key=(char*)malloc(500);
     char* value=(char*)malloc(500);
     int i;
     int pipe_len=16;
     time_t now,then;
     now = time(0);
     while(num>0){
          for(i=0;i<pipe_len;i++){
             scanf("%s %s",key,value);
             pipe_set(sc,key,value);
         }
         pipe_getAllReply(sc);
	 num-=pipe_len;
     }
     then = time(0);
     printf("time taken: %lld\n",then - now);
     single_disconnect(sc);
}*/


int main(){
//init_global();
  pipe_example("192.168.1.21",6379,"now");
//test_with_multiple_threads("192.168.1.21",6667);

 return 0;
}
