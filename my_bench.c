#include"my_bench.h"
#include"connect.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<hiredis/hiredis.h>

static void *__db_function(void* thread_struct);
static void *__thread_pipeline_test(void* thread_struct);

/*
*input: add any valid filename
*
*/
void pipe_example (char* in_ip, int in_port, char* filename) {
     int port=in_port;
     char* ip=in_ip;
     FILE* fp;
     singleClient* sc = single_connect(port,ip);
     if((fp=freopen(filename,"a",stdin))==NULL){
         fprintf(stderr,"error redirection\n");
     }
     char* key=(char*)malloc(500);
     char* value=(char*)malloc(500);
     char* revalue=(char*)malloc(500);
     puts("hehe");

     pipe_getReply(sc,revalue);
     pipe_set(sc,"testk","testv");
     pipe_set(sc,"testk1","testv1");     

     puts(revalue);

     pipe_get(sc,"testk");
     pipe_get(sc,"testk1");

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




#define PIPE_TEST_COUNT 20

void* __thread_pipeline_test(void *thread_input) {
    char * ip = ((thread_struct*)thread_input)->in_ip;
    int port = ((thread_struct*)thread_input)->in_port;
    int my_tid = ((thread_struct*)thread_input)->tid;
    clusterInfo *cluster = connectRedis("192.168.1.22",6667);

    if(cluster == NULL) {
        printf("unable to connect to cluster\n");
    }else {
        printf("connection succeed\n");
    }

    int step = ((thread_struct*)thread_input)->step;

    //three steps before using a cluster mode pipeline
    clusterPipe *mypipe = get_pipeline();
    set_pipeline_count(mypipe,PIPE_TEST_COUNT);
    bind_pipeline_to_cluster(cluster,mypipe);
    
    
    char key[256],value[256];
    int count = 0;
    int init = (my_tid/step - 1)*step;
    printf("tid=%d start=%d end=%d\n",my_tid,init,my_tid);

    for(int i=init;i<my_tid;i++) {
        sprintf(key,"key=%d",i);
        sprintf(value,"value=%d",i);
	//we have a key and value here, so we can use the pipeline
        count++;
        if(count<PIPE_TEST_COUNT){
            cluster_pipeline_set(cluster,mypipe,key,value);
        }else{
            cluster_pipeline_set(cluster,mypipe,key,value);
            count=0;
            cluster_pipeline_flushBuffer(cluster,mypipe);
            int inner = 0;
            for(;inner<PIPE_TEST_COUNT;inner++) {
                redisReply *reply = cluster_pipeline_getReply(cluster,mypipe);
                if(reply == NULL) {
                    printf("NULL reply in %d\n",i);
                }else{
                    freeReplyObject(reply);
                }
            }
            cluster_pipeline_complete(cluster,mypipe);
            reset_pipeline_count(mypipe,PIPE_TEST_COUNT);
        }        
    }
    disconnectDatabase(cluster);    
    return (void*)0;
}

void test_pipeline_with_multiple_threads (char *ip,int port) {
    pthread_t th[16];
    int res, i;
    char local_ip[30];
    strncpy(local_ip,ip,25);
    thread_struct thread_input[16];

    int step = 1000;
    
    for(i=0;i<16;i++) {
        thread_input[i].in_ip = local_ip;
        thread_input[i].in_port = port;
        thread_input[i].tid = (i+1)*step;
        thread_input[i].step = step;
    }

    for(i=0;i<16;i++) { 
        res = pthread_create(&th[i],NULL,__thread_pipeline_test,(void*)(&thread_input[i]));
        if(res!=0) {
            printf("thread fail\n");
            break;
        }
    }

    void* thread_result;

    for(i=0;i<16;i++) { 
    res = pthread_join(th[i],&thread_result);
        if(res!=0) {
            printf("thread join fail\n");
            break;
        }
    }
}
