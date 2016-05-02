#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include <errno.h>

void get(char *key){
    //redisContext* c = redisConnect("172.16.32.209", 6379);
    redisContext* c = redisConnect("172.16.32.209", 7001);
    redisReply* r = (redisReply*)redisCommand(c,"get %s",key);
    if(r->type == REDIS_REPLY_STRING){
        printf("get value = %s",r->str);
    }else{
	    printf("get value = %s \n",r->str);
	}
    freeReplyObject(r);
    redisFree(c);
}

void set(char* key, char* value){
    redisContext* c = redisConnect("172.16.32.211", 7002);
    redisReply* r = (redisReply*)redisCommand(c,"set %s %s",key, value);
    if(r->type == REDIS_REPLY_STRING){
        printf("value = %s",r->str);
    }else{
	   printf("?? value= %s  type = %d\n",r->str,r->type);
	
	}
    freeReplyObject(r);
    redisFree(c);
}
void __process_cluster_str(char* str){
	puts(str); 

}

void __clusterInfo(){
    redisContext* c = redisConnect("172.16.32.211", 7002);
    redisReply* r = (redisReply*)redisCommand(c,"cluster nodes");
	printf("%s\n",r->str);
    char * temp = r->str; 
    printf("%s\n",strstr(temp,"connected"));

}

int main(){
    
    
	__clusterInfo();
	return 0;
}
