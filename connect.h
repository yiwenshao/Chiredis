#ifndef CONNECT_H
#define CONNECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

typedef struct parseArgv{
    char * ip;
    redisContext * context;
    int port;
    int start_slot;
    int end_slot;
    char slots[16384];
}parseArgv;

typedef struct clusterInfo{
    int len;
    char * argv[50];
    parseArgv* parse[50];
    void * slot_to_host[16384];
    redisContext* globalContext;
}clusterInfo;

typedef struct singleClient{
    redisContext* singleContext;
    int port;
    const char* ip;
    int pipe_count;
}singleClient;


clusterInfo* connectRedis(char*ip,int port);
int set(clusterInfo* cluster,const char *key, char *set_in_value,int dunum,int tid);
int __set_nodb(clusterInfo* cluster,const char* key,char* set_in_value);
int __set_withdb(clusterInfo* cluster,const char* key, char* set_in_value, int dbnum,int tid);


int get(clusterInfo*cluster, const char *key, char *get_in_value, int dbnum,int tid);
int __get_withdb(clusterInfo*cluster, const char* key,char*get_in_value,int dbnum,int tid);
int __get_nodb(clusterInfo*cluster, const char* key,char* get_in_value);

void __process_cluster_str(char* str);
clusterInfo* __clusterInfo(redisContext* localContext);

void print_clusterInfo_parsed(clusterInfo* mycluster);
void process_cluterInfo(clusterInfo* mycluster);

void from_str_to_cluster(char * temp, clusterInfo* mycluster);
void __test_slot(clusterInfo* mycluster);
void assign_slot(clusterInfo* mycluster);
void __add_context_to_cluster(clusterInfo* mycluster);
void __remove_context_from_cluster(clusterInfo* mycluster);
void disconnectDatabase(clusterInfo* cluster);
void __global_disconnect(clusterInfo* cluster);

void __set_redirect(char* str);

clusterInfo* __connect_cluster(char* ip, int port);

int flushDb(clusterInfo* cluster);

typedef struct getspace{
     char * getKey;
     int used;
}getspace;

typedef struct setspace{
     char* setKey;
     int used;
}setspace;

//get and set operation can use this global buffer 
getspace global_getspace[100];
setspace global_setspace[100];
void init_global();
singleClient* single_connect(int port,const char* ip);
void pipe_set(singleClient*sc, char*key, char*value);
void pipe_get(singleClient*sc, char*key);
void pipe_getReply(singleClient*sc);
void pipe_getAllReply(singleClient*sc);
void single_disconnect(singleClient* sc);

#endif
