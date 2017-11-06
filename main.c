#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>
#include "my_bench.h"

void disconnect_after_connect(){
    init_global();
    clusterInfo *cluster = connectRedis("127.0.0.1",6667);
    if(cluster != NULL) {
        printf("connected to cluster\n");
    }else{
        printf("panic\n");
    }
    char key[10] = "key";
    char value[10] = "value";
    
    set(cluster,key,value,1,1);
    get(cluster,key,value,1,1);
    printf("get value= %s\n",value);
    
    disconnectDatabase(cluster);
    release_global();
}

int main() {
    int i;
    for(i=0;i<100;i++)
        disconnect_after_connect();
    return 0;
}




