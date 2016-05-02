#ifndef REDIS_H
#define REDIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

#define USER_DEVICE_DB	1
#define USER_KEY_DB		2
#define	DEVICE_STATE_DB	3

#define REPLY_SUCCESS	1
#define REPLY_NULL		4
#define REPLY_ERROR		6

#define DEVICE_LEN		20
#define STATE_LEN		80
#define KEY_LEN			20

#define MAX_CONCURRENCY 3

void connectRedis();
//void selectDatabase(redisContext *c[], const unsigned int db);
void disconnectDatabase();
void set(const char *key, const char *value);
int get(const char *key, char *value);
redisContext * currentConnection[MAX_CONCURRENCY];

typedef struct ipContext{
   char ip[16];
   redisContext * context;
}ipContext;
ipContext global[3];

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

void __process_cluster_str(char* str);
void __clusterInfo();

void print_clusterInfo_parsed(clusterInfo* mycluster);
void process_cluterInfo(clusterInfo* mycluster);

void from_str_to_cluster(char * temp, clusterInfo* mycluster);

//redisContext *userDeviceDB[MAX_CONCURRENCY];
//redisContext *userKeyDB[MAX_CONCURRENCY];
//redisContext *deviceStateDB[MAX_CONCURRENCY];

//unsigned int userDeviceCounter;
//unsigned int userKeyCounter;
//unsigned int deviceStateCounter;






#endif
