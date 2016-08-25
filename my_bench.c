#include"my_bench.h"
#include"connect.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
void *__db_function(void* thread_struct);

/*
*input: add any valid filename
*
*/
void pipe_example (char* filename) {
     int port=6379;
     char* ip="127.0.0.1";
     FILE* fp;
     singleClient* sc = single_connect(port,ip);
     if((fp=freopen(filename,"a",stdin))==NULL){
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



/*

this function is tested, and it is intended to be used in pthread_create
para1: ip
para2: port
para3: thread id

*/

void *__db_function(void* thread_input){
  
  char * ip = ((thread_struct*)thread_input)->in_ip;
  int port = ((thread_struct*)thread_input)->in_port;
  int my_tid = ((thread_struct*)thread_input)->tid;

  /*
  *each thread has its own value and cluster struct.
  */
  char*  value = (char*)malloc(1024*8); 
  char*  key = (char*)malloc(100);

  clusterInfo *cluster = connectRedis(ip,port);

  if(cluster!=NULL){
      printf("connected to cluster\n");
  }
  else{ 
  	printf("cluster==null\n");
	exit(0);
  }

  int sum = 0;
 
  sprintf(key,"key=%d",my_tid);

  sprintf(value,"%s","aaaaa");

  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);


  sprintf(value,"%s","ffffff");
  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s:%s\n",key,value);


  sprintf(value,"%s","aefde");
  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);

  sprintf(value,"%s","abcdefads");
  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);
  
  if(sum == 0)
     printf("all operations succeed!\n");
  else printf("operation fail\n");

  disconnectDatabase(cluster);
}



void test_with_multiple_threads(char*ip,int port) {
 pthread_t th[16];
 int res, i;
 char local_ip[30];
 strncpy(local_ip,ip,25);
 thread_struct thread_input[16];
 for(i=0;i<16;i++) {
    thread_input[i].in_ip = local_ip;
    thread_input[i].in_port = port;
    thread_input[i].tid = i;
 }

 init_global();//shared memory which allows less than 16 threads

 int thread[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
 for(i=0;i<16;i++){ 
   res = pthread_create(&th[i],NULL,__db_function,(void*)(&thread_input[i]));
   if(res!=0){
      printf("thread fail\n");
      break;
   }
 }

 void* thread_result;
 for(i=0;i<16;i++){ 
   res = pthread_join(th[i],&thread_result);
   if(res!=0){
      printf("thread join fail\n");
      break;
     }
  }
}
