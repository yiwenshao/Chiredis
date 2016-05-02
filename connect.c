#include "connect.h"
#include <stdio.h>
#include <hiredis/hiredis.h>
#include <errno.h>


//#define DEBUG

//char *database_ip = "172.16.32.207";
//int port = 6379;

void connectRedis(){
     currentConnection[0] = redisConnect("172.16.32.211",7002);
	 currentConnection[1] = redisConnect("172.16.32.209",7001);
     currentConnection[2] = redisConnect("172.16.32.208",7000);
     strcpy(global[0].ip,"172.16.32.211");
	 global[0].context = currentConnection[0];

     strcpy(global[1].ip,"172.16.32.209");
	 global[1].context = currentConnection[1];

     strcpy(global[2].ip,"172.16.32.208");
	 global[2].context = currentConnection[2];

	 if(currentConnection[0]->err || currentConnection[1]->err||currentConnection[2]->err){
	     redisFree(currentConnection[0]);
	     redisFree(currentConnection[1]);
	     redisFree(currentConnection[2]);
		 printf("connection refused\n");
	 }else{
	     printf("succeed in connecting\n");
	 }
}

/*
void __selectDatabase(redisContext *c, const unsigned int db)
{
	redisReply *reply;
	reply = redisCommand(c, "select %d", db);
	assert(reply != NULL);
	freeReplyObject(reply);

	reply = redisCommand(c, "dbsize");
	assert(reply != NULL);
	if (reply->type == REDIS_REPLY_INTEGER && reply->integer == 0) {
		freeReplyObject(reply);
	} else {
		printf("Database #%d is not empty, flush.\n", db);
		reply = redisCommand(c, "FLUSHDB");
		assert(reply != NULL);
		freeReplyObject(reply);
	}
}


void selectDatabase(redisContext *c[], const unsigned int db) 
{
	int i;
	for (i=0; i<MAX_CONCURRENCY; i++) {
		__selectDatabase(c[i], db);	
	}
}
*/

void  __disconnectDatabase(redisContext *c)
{
	//redisReply *reply;
	//reply = redisCommand(c, "FLUSHDB");
	//assert(reply != NULL);
	//freeReplyObject(reply);
	redisFree(c);
	printf("connection freed\n");
}


void disconnectDatabase()
{
	int i;
	for (i=0; i<MAX_CONCURRENCY; i++) {
		__disconnectDatabase(currentConnection[i]);	
	}
}

void __set(redisContext *c,  const char *key, const char *value){
	redisReply *r = (redisReply *)redisCommand(c, "set %s %s", key, value);
	if (r->type == REDIS_REPLY_STATUS) {
#ifdef DEBUG
		printf("STATUS = %s\n", r->str);
#endif
	}else{
#ifdef DEBUG
	   printf("still error %s\n",r->str);
#endif
	} 
	freeReplyObject(r);
}

void set(const char *key, const char *value)
{
	redisContext *c = currentConnection[0];	
	redisReply *r = (redisReply *)redisCommand(c, "set %s %s", key, value);
	if (r->type == REDIS_REPLY_STRING) {

#ifdef DEBUG
		//printf("value = %s\n", r->str);
#endif
	}else if(r->type == REDIS_REPLY_ERROR && !strncmp(r->str,"MOVED",5)){
#ifdef DEBUG
		printf("set need redirection %s\n", r->str);
#endif
        char *s,*p;
		s = strchr(r->str,' ');
		s++;
		p = strchr(s,' ');
		p++;
		s = strchr(p,':');
		s++;
		//printf("port = %s  p = %s\n",s,p);
		char newIp[s-p+3];
		strncpy(newIp,p,s-p-1);
#ifdef DEBUG
		printf("get new ip = %s len = %d\n",newIp,strlen(newIp));
#endif
		int iter;
        for(iter=0;iter<3;iter++){
		       if(!strcmp(global[iter].ip,newIp)){
				   c = global[iter].context;
				   __set(c,key,value);
				   break;
			   }else{
			       // printf("get iter=%d not found new ip =%s len=%d  cur=%s\n",iter,newIp,strlen(newIp),global[iter].ip); 
			   }
		}

	}else if(r->type == REDIS_REPLY_STATUS){
#ifdef DEBUG
		printf("STATUS = %s\n",r->str);
#endif
	}else{
	   printf("set error\n");
	
	}
	freeReplyObject(r);
}

int __get(redisContext*c, const char *key, char *value){
	redisReply *r = (redisReply *)redisCommand(c, "get %s", key);
#ifdef DEBUG
	printf("type: %d\n", r->type);
#endif
	if (r->type == REDIS_REPLY_STRING) {
#ifdef DEBUG

		printf("%s = %s\n", key, r->str);
#endif
		int len = strlen(r->str);
		strcpy(value, r->str);
		freeReplyObject(r);
		return REPLY_SUCCESS;
	}else if(r->type == REDIS_REPLY_NIL){

		strcpy(value,"nil");
#ifdef DEBUG
		printf("value = nil\n");
		return -1;
#endif
	}else{
	    
		strcpy(value,"NULL");
#ifdef DEBUG
		printf("still error\n");
#endif
		return -1;
	}
}

int get(const char *key, char *value)
{
	redisContext * c = currentConnection[0];
	redisReply *r = (redisReply *)redisCommand(c, "get %s", key);
#ifdef DEBUG
	printf("type: %d\n", r->type);
#endif
	if (r->type == REDIS_REPLY_STRING) {
#ifdef DEBUG
		printf("%s = %s\n", key, r->str);
#endif  
		int len = strlen(r->str);
		strcpy(value, r->str);
		freeReplyObject(r);
		return REPLY_SUCCESS;
	}else if (r->type == REDIS_REPLY_NIL) {
		strcpy(value,"nil");
		freeReplyObject(r);
		return REPLY_NULL;
	} else if(r->type == REDIS_REPLY_ERROR && !strncmp(r->str,"MOVED",5)){
#ifdef DEBUG
		printf("get need redirection  %s\n",r->str);
#endif
        char *s,*p;
		s = strchr(r->str,' ');
		s++;
		p = strchr(s,' ');
		p++;
		s = strchr(p,':');
		s++;
		char newIp[s-p+1];
		strncpy(newIp,p,s-p-1);
#ifdef DEBUG
		puts(newIp);
#endif
		int iter;
		int returnValue;
		
        for(iter=0;iter<3;iter++){
		       if(!strcmp(newIp,global[iter].ip)){
				   c = global[iter].context;
				 //  printf("get find\n");
				   returnValue = __get(c,key,value);
				   break;
			   }else{
			       //printf("get iter=%d not found new ip =%s  cur=%s\n",iter,newIp,global[iter].ip);
			   }
		} 
	    freeReplyObject(r);
	    return REPLY_ERROR;
	} else {
		return -1;
	}
}

void __process_cluster_str(char* str){
    char *temp = str;
    char * argv[50];
    int count = 0;
    char* point;
    int copy_len = 0;

    while((point=strchr(temp,'\n'))!=NULL){
	    copy_len = point-temp+1;
	    argv[count] = (char*)malloc(copy_len);
	    strncpy(argv[count],temp,copy_len - 1);
	    argv[count][copy_len-1] = '\0';
	    count++;
	    temp = point+1;
	}

	argv[count] = temp;
    int i;
    for(i=0;i<=count;i++){
        puts(argv[i]);
    }
}

void __clusterInfo(){
    char *argv[20];
    int count = 0;
    redisContext* c = redisConnect("172.16.32.211", 7002);
    redisReply* r = (redisReply*)redisCommand(c,"cluster nodes");
	printf("%s\n",r->str);
    __process_cluster_str(r->str);

}







