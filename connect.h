#ifndef CONNECT_H
#define CONNECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

/*
*parseArgv represents one single redis instance in a redis cluster.It's simply a formatted version of one line of the response of cluster nodes
*
*/
typedef struct parseArgv{
    //ip address of the redis instance
    char * ip;
    //port of the redis instance
    int port;
    //one connection per instance
    redisContext * context;
    //the instance have an starting slot and an ending slot.
    int start_slot;
    int end_slot;
    //if slots[i] equals 1, it means this instance owns that slot,otherwise the instance doesn't own the slot.
    char slots[16384];
}parseArgv;

/*
*this structure contains all the information needed to communicate with a redis cluster.
*
*/
typedef struct clusterInfo{
    //size of the cluster
    int len;
    //one line of information from the response of cluster nodes
    char * argv[500];
    //formatted version of the above information
    parseArgv* parse[500];
    //each slot points to a redis instance, a redis instance is represented by a parseArgv structure.
    void * slot_to_host[16384];
    //globalContext is used to send 'cluster nodes' and receive the response
    redisContext* globalContext;
}clusterInfo;

/*
*use a single ip and port to connect to a redis cluster.
*this function use the single ip and port to connect to one redis instance in the redis cluster,
*send the command 'cluster nodes',receive the response, construct clusterInfo, and the we are free to use set and get
*/
clusterInfo* connectRedis(char*ip,int port);

int set(clusterInfo* cluster,const char *key, char *set_in_value,int dunum,int tid);
int __set_nodb(clusterInfo* cluster,const char* key,char* set_in_value);
int __set_withdb(clusterInfo* cluster,const char* key, char* set_in_value, int dbnum,int tid);


int get(clusterInfo*cluster, const char *key, char *get_in_value, int dbnum,int tid);
int __get_withdb(clusterInfo*cluster, const char* key,char*get_in_value,int dbnum,int tid);
int __get_nodb(clusterInfo*cluster, const char* key,char* get_in_value);

void print_clusterInfo_parsed(clusterInfo* mycluster);




void assign_slot(clusterInfo* mycluster);

void __remove_context_from_cluster(clusterInfo* mycluster);

void disconnectDatabase(clusterInfo* cluster);


void __set_redirect(char* str);



int flushDb(clusterInfo* cluster);


/*
*getspace and set space are initialized using init_global. currently there are 100 slot. each call to set or get method with their tid 
*should get their share of the allocated space, without the need to allocated space for every call to set and get.
*/
typedef struct getspace{
     char * getKey;
     int used;
}getspace;

typedef struct setspace{
     char* setKey;
     int used;
}setspace;

getspace global_getspace[100];
setspace global_setspace[100];

void init_global();

/*
*use singleContext to connect to one redis instance.and use the conresponding functions to try pipeline with single redis instance. This structure does not 
*support redis cluster.
*related functions:
*single_connect, pipe_set,pipe_get,pipe_getReply,pipe_getAllReply,single_disconnect.
*/

typedef struct singleClient{
    redisContext* singleContext;
    int port;
    const char* ip;
    int pipe_count;
}singleClient;

singleClient* single_connect(int port,const char* ip);
void pipe_set(singleClient*sc, char*key, char*value);
void pipe_get(singleClient*sc, char*key);
void pipe_getReply(singleClient*sc,char* revalue);
void pipe_getAllReply(singleClient*sc);
void single_disconnect(singleClient* sc);

#endif
