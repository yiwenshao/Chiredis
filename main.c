#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>
#include "my_bench.h"

int main(){
//init_global();
//pipe_example("192.168.1.21",6379,"now");
//test_with_multiple_threads("192.168.1.22",6667);
    clusterInfo *cluster = connectRedis("192.168.1.22",6667);
    if(cluster == NULL) {
        printf("unable to connect to cluster\n");
    }else{
        printf("connection succeed\n");
    }
    
    clusterPipe* mypipe = get_pipeline();
    set_pipeline_count(mypipe,20);

    bind_pipeline_to_cluster(cluster,mypipe);

/*
    cluster_pipeline_get(cluster,mypipe,"k1","v1");   
    cluster_pipeline_get(cluster,mypipe,"k2","v2"); 
    cluster_pipeline_get(cluster,mypipe,"k3","v3");
    cluster_pipeline_get(cluster,mypipe,"k4","v4");   
    cluster_pipeline_get(cluster,mypipe,"k5","v5"); 
    cluster_pipeline_get(cluster,mypipe,"k6","v6");
    cluster_pipeline_get(cluster,mypipe,"k7","v7");   
    cluster_pipeline_get(cluster,mypipe,"k8","v8"); 
    cluster_pipeline_get(cluster,mypipe,"k9","v9");
    cluster_pipeline_get(cluster,mypipe,"k10","v10");   
    cluster_pipeline_get(cluster,mypipe,"k11","v11"); 
    cluster_pipeline_get(cluster,mypipe,"k12","v12");
    cluster_pipeline_get(cluster,mypipe,"k13","v13");   
    cluster_pipeline_get(cluster,mypipe,"k14","v14"); 
    cluster_pipeline_get(cluster,mypipe,"k15","v15");
    cluster_pipeline_get(cluster,mypipe,"k16","v16");   
    cluster_pipeline_get(cluster,mypipe,"k17","v17"); 
    cluster_pipeline_get(cluster,mypipe,"k18","v18");
    cluster_pipeline_get(cluster,mypipe,"k19","v19");   
    cluster_pipeline_get(cluster,mypipe,"k20","v20"); 
*/

    cluster_pipeline_get(cluster,mypipe,"k1");   
    cluster_pipeline_get(cluster,mypipe,"k2"); 
    cluster_pipeline_get(cluster,mypipe,"k3");
    cluster_pipeline_get(cluster,mypipe,"k4");   
    cluster_pipeline_get(cluster,mypipe,"k5"); 
    cluster_pipeline_get(cluster,mypipe,"k6");
    cluster_pipeline_get(cluster,mypipe,"k7");   
    cluster_pipeline_get(cluster,mypipe,"k8"); 
    cluster_pipeline_get(cluster,mypipe,"k9");
    cluster_pipeline_get(cluster,mypipe,"k10");   
    cluster_pipeline_get(cluster,mypipe,"k11"); 
    cluster_pipeline_get(cluster,mypipe,"k12");
    cluster_pipeline_get(cluster,mypipe,"k13");   
    cluster_pipeline_get(cluster,mypipe,"k14"); 
    cluster_pipeline_get(cluster,mypipe,"k15");
    cluster_pipeline_get(cluster,mypipe,"k16");   
    cluster_pipeline_get(cluster,mypipe,"k17"); 
    cluster_pipeline_get(cluster,mypipe,"k18");
    cluster_pipeline_get(cluster,mypipe,"k19");   
    cluster_pipeline_get(cluster,mypipe,"k20");
    
    cluster_pipeline_flushBuffer(cluster,mypipe);

    int i;

    for (i=0;i<20;i++) {
        redisReply* reply =  cluster_pipeline_getReply(cluster,mypipe);
        if(reply == NULL) {
            printf("NULL reply\n");
            continue;
        }
        printf("%s\n",reply->str);
        freeReplyObject(reply);
    }

    cluster_pipeline_complete(cluster,mypipe);

    printf("set a new pipeline hahahaha .......\n"); 

    set_pipeline_count(mypipe,10);
       
    cluster_pipeline_get(cluster,mypipe,"k11"); 
    cluster_pipeline_get(cluster,mypipe,"k12");
    cluster_pipeline_get(cluster,mypipe,"k13");   
    cluster_pipeline_get(cluster,mypipe,"k14"); 
    cluster_pipeline_get(cluster,mypipe,"k15");
    cluster_pipeline_get(cluster,mypipe,"k16");   
    cluster_pipeline_get(cluster,mypipe,"k17"); 
    cluster_pipeline_get(cluster,mypipe,"k18");
    cluster_pipeline_get(cluster,mypipe,"k19");   
    cluster_pipeline_get(cluster,mypipe,"k20");

    cluster_pipeline_flushBuffer(cluster,mypipe);
    for (i=0;i<10;i++) {
        redisReply* reply =  cluster_pipeline_getReply(cluster,mypipe);
        if(reply == NULL) {
            printf("NULL reply\n");
            continue;
        }
        printf("%s\n",reply->str);
        freeReplyObject(reply);
    }

    cluster_pipeline_complete(cluster,mypipe);
    disconnectDatabase(cluster); 
    return 0;
}



