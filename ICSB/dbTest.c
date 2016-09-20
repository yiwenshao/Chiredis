#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include"dbTest.h"

static void* __thread_worker(void *thread_input) {

    int my_tid = ((thread_struct*)thread_input)->tid;
    //connect
    functionStruct* localFunction = ((thread_struct*)thread_input)->function;
    void *thread_prepare_out = localFunction->prepare(thread_input,localFunction->prepare_in);   
    printf("this tid = %d\n",my_tid); 

    benchmarkConfig *bc = ((thread_struct*)thread_input)->bc;
    //use the benchmark; 
    benchmarkInfo *benchmark = initBenchmark(bc->totalCount);
    benchmark = loadData(benchmark);
    kvPair *tempPair;
    char *key,*value;
    long long total_start = s_time();

    int i;
    for(i=0;i<bc->totalCount;i++) {
        tempPair = getKvPair(benchmark);
        long long start = us_time();
        localFunction->operation_out = localFunction->operation((void*)tempPair,thread_input,thread_prepare_out);
        long long end = us_time();
        addDuration(benchmark,end-start);
    }

    long long total_end = s_time();
    localFunction->end_out = localFunction->end(thread_input,localFunction->end_in);
    char temp[11] = "pip";
    sprintf(temp+3,"%d",my_tid);
    //after benchmark
    setFileName(benchmark,temp);
    flushResults(benchmark);
    
    return (void*)0;
}

static void assert_functionStruct(functionStruct *function) {
    assert(function->init != NULL);
    assert(function->prepare != NULL);
    assert(function->operation != NULL);
    assert(function->end != NULL);
}

void startTest (char *ip,int port,functionStruct *function) {
    assert(function);
    benchmarkConfig * bc = init_config();
    int thread_count = bc->threadCount;

    pthread_t *th = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);

    if(th == NULL) {
        printf("malloc fail %s %d\n",__FILE__,__LINE__);
        return ;
    }

    char local_ip[30];

    strncpy(local_ip,ip,25);

    thread_struct *thread_input = (thread_struct*)malloc(sizeof(thread_struct)*thread_count);

    if(thread_input == NULL) {
        printf("malloc fail %s %d\n",__FILE__,__LINE__);
        return ;
    }

    int i;
    for(i=0;i<thread_count;i++) {
        thread_input[i].in_ip = local_ip;
        thread_input[i].in_port = port;
        thread_input[i].tid = (i+1);
        thread_input[i].bc = bc;
        thread_input[i].function = function;
    }

    //step one: before creating the threads.    
    function->init_out = function->init((void*)0);

    for(i=0;i<thread_count;i++) {
        int res = pthread_create(&th[i],NULL,__thread_worker,(void*)(&thread_input[i]));
        if(res!=0) {
            printf("thread fail\n");
            break;
        }
    }

    void* thread_result;
    for(i=0;i<thread_count;i++) {
        int res = pthread_join(th[i],&thread_result);
        if(res!=0) {
            printf("thread join fail\n");
            break;
        }
    }
}

