#include<stdio.h>
#include<stdlib.h>
#include"dbTest.h"
#include"chiredis/connect.h"
functionStruct* functions;

//TODO:
void *init(void* myinput) {
    printf("init\n");
    init_global();    
    return (void*)0;
}


//TODO:
void *prepare(void *thread_input,void* myinput){
    printf("prepare\n");
    clusterInfo *cluster = connectRedis("192.168.1.22",6667);
    if(cluster == NULL)
        return NULL;
    //this will be transmitted to operation 
    return (void*)cluster;
}


//TODO:
void *operation(void *kv,void* thread_input,void *prepare_out) {
    kvPair* lkv = (kvPair*)kv;
    int tid = ((thread_struct*)thread_input)->tid;
    clusterInfo *cluster = (clusterInfo*)prepare_out;
    set(cluster,lkv->key,lkv->value,1,tid);
    char * re  = (char*)malloc(100);
    get(cluster,lkv->key,re,1,tid);
    printf("re = %s\n",re);
    return (void*)0;
}


//TODO:
void *end(void *thread_input,void* input){
    printf("end\n");
    return (void*)0;
}


int main() {
    functions= (functionStruct*)malloc(sizeof(functionStruct));
    if(functions == NULL){
        printf("error malloc\n");
        return -1;
    }

    functions->init = init;
    functions->prepare = prepare;
    functions->operation = operation;
    functions->end = end;
    
    startTest("192.168.1.1",6666,functions);

    return 0;
}
