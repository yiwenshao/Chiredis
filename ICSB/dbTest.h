#ifndef DB_TEST
#define DB_TEST
#include"benchmarkHelp.h"

typedef struct functionStruct {
    void* (*init)(void*);
    void* init_in, *init_out;
    void* (*prepare)(void*,void*);
    void* prepare_in, *prepare_out;
    void* (*operation)(void*,void*,void*);
    void* operation_in,*operation_out;
    void* (*end)(void*,void*);
    void* end_in, *end_out;
} functionStruct;


typedef struct thread_struct {
    char* in_ip;
    int in_port;
    int tid;
    benchmarkConfig *bc;
    functionStruct *function;
} thread_struct;

void startTest (char *ip,int port,functionStruct *function);


#endif
