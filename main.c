#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>
#include "my_bench.h"

int main(){
//init_global();
//pipe_example("192.168.1.21",6379,"now");
test_with_multiple_threads("192.168.1.22",6667);
/*  clusterInfo *cluster = connectRedis("192.168.1.22",6667);
    if(cluster == NULL) {
        printf("unable to connect to cluster\n");
    }else{
        printf("connection succeed\n");
    }
   
    disconnectDatabase(cluster); */
    return 0;
}
