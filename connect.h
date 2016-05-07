#ifndef CONNECT_H
#define CONNECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

void connectRedis(char*ip,int port);
int set(const char *key, const char *set_in_value,int dunum);
int get(const char *key, char *value, int dbnum);
redisContext * globalContext;

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
}clusterInfo;

clusterInfo* globalCluster;

void __process_cluster_str(char* str);
clusterInfo* __clusterInfo();

void print_clusterInfo_parsed(clusterInfo* mycluster);
void process_cluterInfo(clusterInfo* mycluster);

void from_str_to_cluster(char * temp, clusterInfo* mycluster);
void __test_slot(clusterInfo* mycluster);
void assign_slot(clusterInfo* mycluster);
void __add_context_to_cluster(clusterInfo* mycluster);
void __remove_context_from_cluster(clusterInfo* mycluster);
void disconnectDatabase();
void __global_disconnect();

void __set_redirect(char* str);
int __get_nodb(const char* key,char* value);
char* value;
char* globalSetKey;
char* globalGetKey;
void __connect_cluster(char* ip, int port);
int __set_nodb(const char* key,const char* set_in_value);
int __set_withdb(const char* key, const char* set_in_value, int dbnum);
int __get_withdb(const char* key,char* value,int dbnum);
int flushDb();
#endif
