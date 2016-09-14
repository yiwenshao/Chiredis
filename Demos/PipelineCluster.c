#include<stdio.h>
#include"chiredis/connect.h"

int main(){
    //STEP ONE: use an ip and port to connect to a redis cluster 
    clusterInfo *cluster = connectRedis("192.168.1.22",6667);

    if (cluster == NULL) {
        printf("unable to connect to cluster\n");
    } else {
        printf("connection succeed\n");
    }

    //STEP TWO: do three things to prepare for a pipeline transaction
        //1. get a pipeline
        //2. set a pipeline count, which a the total number of set/get operations you can perform in this transaction. In this example,
            //the count is set to 40. Therefore, you must perform exactly 40 set/get operations.
        //3. bind_pipeline_to_cluster 
     
    clusterPipe* mypipe = get_pipeline();
    set_pipeline_count(mypipe,40);
    bind_pipeline_to_cluster(cluster,mypipe);

    int i;
    char key[100],value[100];

    //STEP THREE:use to function cluster_pipeline_set/cluster_pipeline_get to operate on redis cluster.
    //20 set and 20 get equals 40.
    for(i=0;i<20;i++) {
        sprintf(key,"key=%d",i);
        sprintf(value,"value=%d",i);
        cluster_pipeline_set(cluster,mypipe,key,value);
        cluster_pipeline_get(cluster,mypipe,key);
    }

    //STEP FOUR: flush the pipeline buffer
    cluster_pipeline_flushBuffer(cluster,mypipe);
    
    //STEP FIVE: get the replies. Since we send 40 commands, we must explicitly get 40 replies here.
    for (i=0;i<40;i++) {
        redisReply* reply =  cluster_pipeline_getReply(cluster,mypipe);
        if(reply == NULL) {
            printf("NULL reply\n");
            continue;
        }
        if(reply->type == REDIS_REPLY_NIL) {
            printf("nil\n");
        }else{
            printf("%s\n",reply->str);
        }
        freeReplyObject(reply);
    }
    
    //STEP SIX: check whether this transaction succeeded. if it succeeded, nothing will happen, otherwise the program aborts.
    cluster_pipeline_complete(cluster,mypipe);

    //What if you want to start another pipeline transaction? You do not need to start from scratch, just call 
    // reset_pipeline_count(mypipe,40). you can use numbers other than 40, but numbers less than 80 is recomended.
    //After call the function reset_pipeline_count, you can then continue to call get/set and getReply.

    //Example of another pipeline transaction.
    reset_pipeline_count(mypipe,20);
    for(i=0;i<10;i++){
        sprintf(key,"key2=%d",i);
        sprintf(value,"value2=%d",i);
        cluster_pipeline_set(cluster,mypipe,key,value);
        cluster_pipeline_get(cluster,mypipe,key);
    }
    cluster_pipeline_flushBuffer(cluster,mypipe);
    for(i=0;i<20;i++){
        redisReply* reply = cluster_pipeline_getReply(cluster,mypipe);
        if(reply == NULL) {
            printf("NULL reply\n");
            continue;
        }

        if(reply->type == REDIS_REPLY_NIL){
            printf("nil\n");
        }else {
            printf("%s\n",reply->str);
        }

        freeReplyObject(reply);
    }

    //STEP SEVEN: disconnect
    release_pipeline(mypipe);
    disconnectDatabase(cluster);
    return 0;
}
