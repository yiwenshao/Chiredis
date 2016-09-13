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
  benchmarkConfig *bc;
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
  }else{ 
  	printf("cluster==null\n");
	exit(0);
  }
  benchmarkConfig *bc = ((thread_struct*)thread_input)->bc;
  if(my_tid == 1)
      show_config(bc);

   
  benchmarkInfo* benchmark = initBenchmark(bc->totalCount); 
  benchmark = loadData(benchmark);
  unsigned long count = benchmark->count;
  kvPair* tempPair;
  unsigned long i;
  
  long long total_start = s_time();
  for(i=0;i<count;i++){
      long long start = us_time();
      tempPair = getKvPair(benchmark);
      set(cluster,tempPair->key,tempPair->value,1,my_tid);
      long long end = us_time();
      addDuration(benchmark,end - start);
  }
  long long total_end = s_time();
  char temp[11] = "nor";
  sprintf(temp+3,"%d",my_tid);
  setFileName(benchmark,temp);
  flushResults(benchmark); 
  disconnectDatabase(cluster);
  printf("normal_mod: tid=%d total_time=%lld\n",my_tid,total_end - total_start);
  return (void*)0;
}



void test_normal_with_multiple_threads (char *ip,int port) {
    benchmarkConfig * bc = init_config();
    
    int thread_count = bc->threadCount;
    pthread_t *th = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
    if(th == NULL){
        printf("malloc fail %s %d\n",__FILE__,__LINE__);
        return ;
    }
    int res, i;
    char local_ip[30];
    strncpy(local_ip,ip,25);


    thread_struct *thread_input = (thread_struct*)malloc(sizeof(thread_struct)*thread_count);
    if(thread_input == NULL){
        printf("malloc fail %s %d\n",__FILE__,__LINE__);
        return ;
    }
    
    for(i=0;i<thread_count;i++) {
        thread_input[i].in_ip = local_ip;
        thread_input[i].in_port = port;
        thread_input[i].tid = (i+1);
        thread_input[i].bc = bc;
    }
    init_global();

    for(i=0;i<thread_count;i++) { 
        res = pthread_create(&th[i],NULL,__db_function,(void*)(&thread_input[i]));
        if(res!=0) {
            printf("thread fail\n");
            break;
        }
    }

    void* thread_result;
    for(i=0;i<thread_count;i++) { 
    res = pthread_join(th[i],&thread_result);
        if(res!=0) {
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
    printf("start to connect %d %s %d\n",my_tid,ip,port);
    clusterInfo *cluster = connectRedis(ip,port);
    benchmarkConfig *bc = ((thread_struct*)thread_input)->bc;
    if(my_tid == 1)
        show_config(bc);

    if(cluster == NULL) {
        printf("unable to connect to cluster\n");
    }else {
        //printf("connection succeed\n");
    }

    //three steps before using a cluster mode pipeline
    clusterPipe *mypipe = get_pipeline();
    set_pipeline_count(mypipe,PIPE_TEST_COUNT);
    bind_pipeline_to_cluster(cluster,mypipe); 
    
    //use the benchmark; 
    benchmarkInfo *benchmark = initBenchmark(bc->totalCount);
    benchmark = loadData(benchmark);
    kvPair *tempPair;
    char *key,*value;
    int count = 0;
    unsigned long i;
    unsigned long loop_pipe = bc->totalCount/PIPE_TEST_COUNT;

    long long total_start = s_time();
    for(i=0;i<loop_pipe;i++) {
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
                    printf("NULL reply in %lu\n",i);
                } else {
                    freeReplyObject(reply);
                }
        }
        cluster_pipeline_complete(cluster,mypipe);
        reset_pipeline_count(mypipe,PIPE_TEST_COUNT);
        long long end = us_time();
        addDuration(benchmark,end-start);
    }
    long long total_end = s_time();

    char temp[11] = "pip";
    sprintf(temp+3,"%d",my_tid);
    //after benchmark
    setFileName(benchmark,temp);
    flushResults(benchmark);
    
    disconnectDatabase(cluster);
    printf("pipeline_mode: tid=%d total_time=%lld\n",my_tid,total_end - total_start);
    return (void*)0;
}


void test_pipeline_with_multiple_threads (char *ip,int port) {
    benchmarkConfig * bc = init_config();
    
    int thread_count = bc->threadCount;
    pthread_t *th = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
    if(th == NULL){
        printf("malloc fail %s %d\n",__FILE__,__LINE__);
        return ;
    }
    int res, i;
    char local_ip[30];
    strncpy(local_ip,ip,25);


    thread_struct *thread_input = (thread_struct*)malloc(sizeof(thread_struct)*thread_count);
    if(thread_input == NULL){
        printf("malloc fail %s %d\n",__FILE__,__LINE__);
        return ;
    }
    
    for(i=0;i<thread_count;i++) {
        thread_input[i].in_ip = local_ip;
        thread_input[i].in_port = port;
        thread_input[i].tid = (i+1);
        thread_input[i].bc = bc;
    }

    for(i=0;i<thread_count;i++) { 
        res = pthread_create(&th[i],NULL,__thread_pipeline_test,(void*)(&thread_input[i]));
        if(res!=0) {
            printf("thread fail\n");
            break;
        }
    }

    void* thread_result;
    for(i=0;i<thread_count;i++) { 
    res = pthread_join(th[i],&thread_result);
        if(res!=0) {
            printf("thread join fail\n");
            break;
        }
    }
}



int main(int argc, char ** argv){
    if(argc <2 ){
        printf("argc < 2\n");
        return 0;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    printf("ip=%s port=%d\n",ip,port);

    if(argc >4 ){
        if(strcasecmp(argv[3],"-s")==0){
            if(strcasecmp(argv[4],"pipeline")==0){
                printf("start pipeline test\n");
                test_pipeline_with_multiple_threads(ip,port);
            }else if(strcasecmp(argv[4],"normal")==0){
                printf("start normal test\n");
                test_normal_with_multiple_threads(ip,port);
            }else{
                printf("unknown test %s\n",argv[4]);
            }
        }
    }

    return 0;
}
