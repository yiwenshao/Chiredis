#include<stdio.h>
#include"chiredis/connect.h"

int main(){
    clusterInfo *cluster = connectRedis("192.168.1.22",6667);
    if(cluster == NULL) {
        printf("unable to connect to cluster\n");
    }else{
        printf("connection succeed\n");
    }

    clusterPipe* mypipe = get_pipeline();
    set_pipeline_count(mypipe,40);
    bind_pipeline_to_cluster(cluster,mypipe);
    int i;
    char key[100],value[100];

    for(i=0;i<20;i++) {
        sprintf(key,"key=%d",i);
        sprintf(value,"value=%d",i);
        cluster_pipeline_set(cluster,mypipe,key,value);
        cluster_pipeline_get(cluster,mypipe,key);
    }
    cluster_pipeline_flushBuffer(cluster,mypipe);
    
    for (i=0;i<40;i++) {
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

