#include<stdio.h>
#include<stdlib.h>
#include"dbTest.h"

//TODO:
void *init(void* input) {
    printf("init\n");    
    return (void*)0;
}


//TODO:do something that each thread shoud do to prepare for operation, like connecting to database.
void *prepare(void* input) {
    printf("prepare\n");
    return (void*)0;
}


//TODO:get a kv pair and do the operation
void *operation(void* input) {
    printf("do\n");
    return (void*)0;
}


//TODO:do something after each thread has completed operation.
void *end(void* input) {
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
