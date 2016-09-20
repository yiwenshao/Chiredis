#include<stdio.h>
#include<stdlib.h>
#include"dbTest.h"
#include"chiredis/connect.h"

typedef struct pipeCluster{
    clusterInfo *cluster;
    clusterPipe *mypipe;
}pipeCluster;


//TODO:
void *init(void* myinput) {
    printf("init\n");
    return (void*)0;
}


//TODO:
void *prepare(void *thread_input,void* myinput){
    printf("prepare\n");
    thread_struct *ts = (thread_struct*)thread_input;
    int port = ts->in_port;
    char *ip = ts->in_ip;
    clusterInfo *cluster = connectRedis(ip,port);
    if(cluster == NULL)
        return NULL;
    clusterPipe *mypipe = get_pipeline();
    set_pipeline_count(mypipe,20);
    bind_pipeline_to_cluster(cluster,mypipe);

    pipeCluster * pCluster = (pipeCluster*)malloc(sizeof(pipeCluster));
    pCluster->cluster = cluster;
    pCluster->mypipe = mypipe;

    //this will be transmitted to operation 
    return (void*)pCluster;
}


//TODO:
void *operation(void *kv,void* thread_input,void *prepare_out) {

    kvPair* lkv = (kvPair*)kv;
    int tid = ((thread_struct*)thread_input)->tid;
    pipeCluster *pCluster = (pipeCluster*)prepare_out;
    clusterInfo *cluster = pCluster->cluster;
    clusterPipe *mypipe = pCluster->mypipe;   

    char *key , *value;
    key = lkv->key;
    value = lkv->value; 

    int count = 0;
    while(count<20){
        cluster_pipeline_set(cluster,mypipe,key,value);
        count++;
    }

    cluster_pipeline_flushBuffer(cluster,mypipe);

    int inner = 0;
    for(;inner<20;inner++){
        redisReply *reply = cluster_pipeline_getReply(cluster,mypipe);
        if(reply == NULL) {
            printf("NULL reply in hehe\n");
        } else {
            printf("re=%s\n",reply->str);
            freeReplyObject(reply);
        }
    }

    cluster_pipeline_complete(cluster,mypipe);
    reset_pipeline_count(mypipe,20);

    return (void*)0;
}

//TODO:
void *end(void *thread_input,void* prepare_out){
    printf("end\n");
    clusterInfo *cluster = ((pipeCluster*)prepare_out)->cluster;
    disconnectDatabase(cluster);
    return (void*)0;
}


int main(int argc, char ** argv) {
    if(argc < 2){
        printf("argc < 2\n");
    }
    
    char * ip = argv[1];
    int port = atoi(argv[2]);

    functionStruct *functions= (functionStruct*)malloc(sizeof(functionStruct));
    if(functions == NULL){
        printf("error malloc\n");
        return -1;
    }

    functions->init = init;
    functions->prepare = prepare;
    functions->operation = operation;
    functions->end = end;
    
    startTest(ip,port,functions);
    return 0;
}
