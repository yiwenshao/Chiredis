#include "connect.h"
#include <stdio.h>
#include <hiredis/hiredis.h>
#include <errno.h>
#include "crc16.h"
#include <string.h>

#define DEBUG
void connectRedis(char* ip, int port){
     globalContext = redisConnect("172.16.32.211",7002);
	 if(globalContext->err){
	     redisFree(globalContext);
		 printf("global connection refused\n");
	 }else{
	     printf("succeed in global connecting\n");
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

/*
*once we meet indirection, this function help parse the information
*/
void __set_redirect(char* str){
	char *slot,*ip,*port;
	slot = strchr(str,' ');
	slot++;
	ip = strchr(slot,' ');
	ip++;
	port = strchr(ip,':');
	port++;
	//s means store
	char* s_slot, *s_ip, *s_port;
        s_slot = (char*)malloc(ip-slot);
	strncpy(s_slot,slot,ip-slot-1);
	s_slot[ip-slot-1]='\0';

	s_ip = (char*)malloc(port-ip);
	strncpy(s_ip,ip,port-ip-1);
	s_ip[port-ip-1]='\0';

	s_port = (char*)malloc(sizeof(port)+1);
	strcpy(s_port,port);
	printf("get new ip = %s slot=%s port =%s\n",s_ip,s_slot,s_port);
	free(s_ip);
	free(s_slot);
	free(s_port);
}
void set(const char *key, const char *value){

	redisContext *c = globalContext;
	int myslot;
	myslot = crc16(key,strlen(key)) & 16383;
	printf("slot calculated= %d\n",myslot);

	redisReply *r = (redisReply *)redisCommand(c, "set %s %s", key, value);
	if (r->type == REDIS_REPLY_STRING) {
#ifdef DEBUG
		printf("value = %s\n", r->str);
#endif
	}else if(r->type == REDIS_REPLY_ERROR && !strncmp(r->str,"MOVED",5)){
		printf("set need redirection %s\n", r->str);
		__set_redirect(r->str);

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
	redisContext * c = globalContext;
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

clusterInfo* __clusterInfo(){
    //redisContext* c = redisConnect("172.16.32.211", 7002);
    redisContext *c = globalContext;
    redisReply* r = (redisReply*)redisCommand(c,"cluster nodes");
	printf("the raw return value = %s\n",r->str);
    clusterInfo* mycluster = (clusterInfo*)malloc(sizeof(clusterInfo));
    globalCluster = mycluster;
    from_str_to_cluster(r->str,mycluster);
    process_cluterInfo(mycluster);
    print_clusterInfo_parsed(mycluster);
    return mycluster;
}


void process_cluterInfo(clusterInfo* mycluster){
    //determine the time of iteration
    int len = mycluster->len;

    printf("len = %d\n",mycluster->len);
    int len_ip=0;
    int len_port=0;
    char* temp;
    char* ip_start, *port_start, *port_end,*connect_start,*slot_start,*slot_end;

    char* temp_slot_start;
    char* temp_port;

    int i=0;
    for(;i<len;i++){
        temp = ip_start = port_start = port_end = connect_start = slot_start = slot_end = temp_slot_start = temp_port = NULL;

        temp = mycluster->argv[i];

        //parse ip
        ip_start = strchr(temp,' ');
        temp = ip_start+1;
        port_start = strchr(temp,':');
        temp = port_start+1;
        port_end = strchr(temp,' ');
        ip_start++;
        len_ip = port_start - ip_start;
        mycluster->parse[i] = (parseArgv*)malloc(sizeof(parseArgv));//do not forget
        mycluster->parse[i]->ip = (char*)malloc(len_ip + 1);

        strncpy(mycluster->parse[i]->ip,ip_start,len_ip);
        mycluster->parse[i]->ip[len_ip]='\0';

        //parsePort
        port_start++;
        len_port = port_end - port_start;
        temp_port = (char*)malloc(len_port+1);
        strncpy(temp_port,port_start,len_port);
        temp_port[len_port]='\0';
        mycluster->parse[i]->port = atoi(temp_port);

        connect_start = strstr(port_end,"connected");
        connect_start = strchr(connect_start,' ');

        slot_start = connect_start+1;
        slot_end = strchr(slot_start,'-');
        slot_end++;

        temp_slot_start = (char*)malloc(slot_end-slot_start);
        strncpy(temp_slot_start,slot_start,slot_end-slot_start-1);
        temp_slot_start[slot_end-slot_start-1]='\0';

        mycluster->parse[i]->start_slot = atoi(temp_slot_start);
        mycluster->parse[i]->end_slot = atoi(slot_end);

        free(temp_slot_start);
    }
}

void print_clusterInfo_parsed(clusterInfo* mycluster){
    int len = mycluster->len;
    int i;
    for(i=0;i<len;i++){
        printf("node %d info\n",i);
        printf("original command: %s\n",mycluster->argv[i]);
        printf("ip: %s, port: %d\n",mycluster->parse[i]->ip,mycluster->parse[i]->port);
        printf("slot_start= %d, slot_end = %d\n",mycluster->parse[i]->start_slot,\
	             mycluster->parse[i]->end_slot);
    }
}

void from_str_to_cluster(char * temp, clusterInfo* mycluster){
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
    for(i = 0;i<count;i++){
        mycluster->argv[i] = argv[i];
    }
    mycluster->len = count;
}

void __test_slot(clusterInfo* mycluster){
    int slot[] = {0,5460,5461,10922,10923,16383};
    int len = sizeof(slot) / sizeof(int);
    int i=0;
    for(i=0;i<len;i++){
        printf("slot = %d info: ip = %s, port = %d.\n",slot[i],((parseArgv*)mycluster->slot_to_host[slot[i]])->ip, ((parseArgv*)mycluster->slot_to_host[slot[i]])->port);
    }
}

void assign_slot(clusterInfo* mycluster){
    int len = mycluster->len;
    int i;
    int count = 0;
    for(i=0;i<len;i++){
        int start = mycluster->parse[i]->start_slot;
        int end = mycluster->parse[i]->end_slot;
        int j=0;
        for(j=start;j<=end;j++){
            mycluster->slot_to_host[j] = (void*)(mycluster->parse[i]);
            count++;
        }
    }
    printf("count = %d \n",count);
}

/*
*This function give each node in the cluster a context connection
*/
void __add_context_to_cluster(clusterInfo* mycluster){
   int len = mycluster-> len;
   int i = 0;
   redisContext * tempContext;
   
   for(i=0;i<len;i++){
       tempContext = redisConnect((mycluster->parse[i])->ip,(mycluster->parse[i])->port);
       if(tempContext->err){
          printf("connection refused in __add_contect_to_cluster\n");
	  redisFree(tempContext);
	  return;
       }else{
          (mycluster->parse[i])->context;
       }
   }
   printf("%d connections established!!\n",len);
}


void __remove_context_from_cluster(clusterInfo* mycluster){
   int len = mycluster-> len;
   int i = 0;
   redisContext * tempContext;
   
   for(i=0;i<len;i++){
      redisFree(mycluster->parse[i]->context);
   }
   printf("%d connections closed!!\n",len);
}


void disconnectDatabase(){
    __global_disconnect();

}

/*
*The global context is used to inquery cluster information
*/
void __global_disconnect(){
     redisFree(globalContext);
     printf("global context freed\n");
}

     

