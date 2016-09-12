#include"chiredis/connect.h"
#include"benchmarkHelp.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<hiredis/hiredis.h>

typedef struct thread_struct {
  char* in_ip;
  int in_port;
  int tid;
  int step;
} thread_struct;

static void *__db_function(void* thread_struct);
static void *__thread_pipeline_test(void* thread_struct);

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

  clusterInfo *cluster = connectRedis(ip,port);

  if(cluster!=NULL){
      printf("connected to cluster\n");
  }
  else{ 
  	printf("cluster==null\n");
	exit(0);
  }
   
  benchmarkInfo* benchmark = initBenchmark(1); 
  benchmark = loadData(benchmark);
  unsigned long count = benchmark->count;
  kvPair* tempPair;
  for(unsigned long i=0;i<count;i++){
      long long start = us_time();
      tempPair = getKvPair(benchmark);
      set(cluster,tempPair->key,tempPair->value,1,my_tid);
      long long end = us_time();
      addDuration(benchmark,end - start);
  }
  char temp[11] = "123";
  sprintf(temp+3,"%d",my_tid);
  setFileName(benchmark,temp);
  disconnectDatabase(cluster);
  flushResults(benchmark); 
  printf("success\n");
}

/*
*this test uses interface with db number and globally shared space.
*/
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
    clusterInfo *cluster = connectRedis("192.168.0.3",6667);

    if(cluster == NULL) {
        printf("unable to connect to cluster\n");
    }else {
        printf("connection succeed\n");
    }

    //three steps before using a cluster mode pipeline
    clusterPipe *mypipe = get_pipeline();
    set_pipeline_count(mypipe,PIPE_TEST_COUNT);
    bind_pipeline_to_cluster(cluster,mypipe); 
    
    //use the benchmark; 
    benchmarkInfo *benchmark = initBenchmark(500000);
    benchmark = loadData(benchmark);
    kvPair *tempPair;
    char *key,*value;
    int count = 0;
     
    for(int i=0;i<25000;i++) {
        count = 0;
        long long start = us_time();
        while(count<PIPE_TEST_COUNT){
            tempPair = getKvPair(benchmark);
            key = tempPair->key;
            value = tempPair->value;
            cluster_pipeline_set(cluster,mypipe,key,value);
            count++;
        }
        cluster_pipeline_flushBuffer(cluster,mypipe);
        int inner = 0;
        for(;inner<PIPE_TEST_COUNT;inner++){
                redisReply *reply = cluster_pipeline_getReply(cluster,mypipe);
                if(reply == NULL) {
                    printf("NULL reply in %d\n",i);
                } else {
                    freeReplyObject(reply);
                }
        }
        cluster_pipeline_complete(cluster,mypipe);
        reset_pipeline_count(mypipe,PIPE_TEST_COUNT);
        long long end = us_time();
        addDuration(benchmark,end-start);
    }

    char temp[11] = "123";
    sprintf(temp+3,"%d",my_tid);
    //after benchmark
    setFileName(benchmark,temp);
    flushResults(benchmark);
    
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

int main(){
    //test_with_multiple_threads("192.168.1.22",6667);
    test_pipeline_with_multiple_threads("192.168.1.22",6667);
    return 0;
}
