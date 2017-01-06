#include<stdio.h>
#include<stdlib.h>
#include"dbTest.h"

//TODO:
void *init(void* input) {
    printf("init\n");    
    thread_struct *ts = ((thread_struct*)input);
    int port = ts->in_port;
    char* ip = ts->in_ip;
    printf("port = %d, ip = %s\n",port,ip);

    return (void*)0;
}


//TODO:do something that each thread shoud do to prepare for operation, like connecting to database.
void *prepare(void* thread_input,void *notused) {
    printf("prepare\n");
    thread_struct *ts = ((thread_struct*)thread_input);
    int tid = ts->tid;
    int port = ts->in_port;
    char* ip = ts->in_ip;
    printf("tid = %d, port = %d, ip = %s\n",tid,port,ip);

    return (void*)0;
}


//TODO:get a kv pair and do the operation
void *operation(void* kv,void* unused2,void* unused3) {
    kvPair* lkv = (kvPair*)kv;    

    char *key , *value;
    key = lkv->key;
    value = lkv->value;
    
    printf("key=%s  value=%s\n",key,value);

    return (void*)0;
}


//TODO:do something after each thread has completed operation.
void *end(void* input,void* unused1) {
    printf("end\n");
    return (void*)0;
}


int main() {
    functionStruct* functions = (functionStruct*)malloc(sizeof(functionStruct));

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
