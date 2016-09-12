#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"hiredis/hiredis.h"
#include"chiredis/connect.h"

void disconnect_after_connect(){
    init_global();
    clusterInfo *cluster = connectRedis("192.168.1.22",6667);
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
    for(i=0;i<1;i++)
        disconnect_after_connect();
    return 0;
}
