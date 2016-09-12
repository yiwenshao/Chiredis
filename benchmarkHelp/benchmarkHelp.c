#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<unistd.h>

#include"benchmarkHelp.h"

char* benchmarkHelpVersion(){
    return "benchmarkHelp version 1.0";
}

/*
Malloc and initialize a benchmarkInfo struct.
*/

benchmarkInfo* initBenchmark(unsigned long init_count){
    //default values for benmarkInfo
    char *init_name ="file";
    unsigned short init_tid = 0;
    unsigned int init_keyLen = 20;
    unsigned int init_valueLen = 20;
    unsigned long init_currentKvPairIndex = 0;
    unsigned long init_currentResultIndex = 0;
    
    benchmarkInfo *mark = (benchmarkInfo*)malloc(sizeof(benchmarkInfo));

    if(mark == NULL) {
        printf("unable to malloc %s %d\n",__FILE__,__LINE__);
    }else{
    //initialize benchmarkInfo with the default values   
        sprintf(mark->name,"%s",init_name);
        mark->tid = init_tid;
        mark->count = init_count;
        mark->keyLen = init_keyLen;
        mark->valueLen = init_valueLen;
        mark->currentKvPairIndex = init_currentKvPairIndex;
        mark->currentResultIndex = init_currentResultIndex;
        mark->flushFile = NULL;

        unsigned long i;
        mark->kvPairToUse = (kvPair**)malloc(sizeof(kvPair*)*init_count);

        if(mark->kvPairToUse == NULL){
            printf("unable to malloc %s %d\n",__FILE__,__LINE__);
            return NULL;
        }
        mark->resultsToUse = (results**)malloc(sizeof(results*)*init_count);

        if(mark->resultsToUse == NULL){
            printf("unable to malloc %s %d\n",__FILE__,__LINE__);
            return NULL;
        }

        for(i=0;i<mark->count;i++){
            //not == !! 
            mark->kvPairToUse[i] = (kvPair*)malloc(sizeof(kvPair));
            mark->resultsToUse[i] = (results*)malloc(sizeof(results));
            if(mark->kvPairToUse[i] == NULL){
                printf("unable to malloc %s %d\n",__FILE__,__LINE__);               
                return NULL;
            }
            if(mark->resultsToUse[i] == NULL){
                printf("unable to malloc %s %d\n",__FILE__,__LINE__);               
                return NULL;
                 
            }
        }
    }
    return mark;
}


//fill in kv pair into the struct benchmarkInfo
benchmarkInfo* loadData(benchmarkInfo* info){
    if(info == NULL){
        printf("load == NULL, unable to load data\n");
        return NULL;
    }

    int keyid = 0;
    int kl = info->keyLen;
    int vl = info->valueLen;
    char keyTemp[kl];
    memset(keyTemp,97,kl);
    char valueTemp[vl];
    memset(valueTemp,97,vl);
    unsigned long i;
    for (i=0;i<info->count;i++) {
        info->kvPairToUse[i]->key = (char*)malloc(sizeof(char)*(kl+1));
        info->kvPairToUse[i]->value = (char*)malloc(sizeof(char)*(vl+1));
        if (info->kvPairToUse[i]->key == NULL || info->kvPairToUse[i]->value == NULL) {
            return NULL;
        }
        memset(info->kvPairToUse[i]->key,97,kl+1);
        memset(info->kvPairToUse[i]->value,97,vl+1);
        sprintf(info->kvPairToUse[i]->key,"%d",keyid);
        sprintf(info->kvPairToUse[i]->value,"%d",keyid);
        keyid++;
    }
    return info;
}

long long us_time() {
    struct timeval tv;
    long long ust;
    gettimeofday(&tv, NULL);
    ust = ((long long)tv.tv_sec)*1000000;
    ust += tv.tv_usec;
    return ust;
}


long long ms_time() {
    struct timeval tv;
    long long mst;

    gettimeofday(&tv, NULL);
    mst = ((long long)tv.tv_sec)*1000;
    mst += tv.tv_usec/1000;
    return mst;
}

long long s_time() {
    struct timeval tv;
    long long st;
    gettimeofday(&tv,NULL);
    st = (long long)tv.tv_sec;
    st += tv.tv_usec/1000000;
    return st;
}



void addDuration(benchmarkInfo *benchmark,long long duration){
    unsigned long count = benchmark->count;
    unsigned long currentResultIndex = benchmark->currentResultIndex;
    if(currentResultIndex < count){
        benchmark->currentResultIndex+=1;
        benchmark->resultsToUse[currentResultIndex]->duration_miliseconds = duration;
    }else{
        printf("result full \n");
    }
}

void freeBenchmark() {

}

void flushResults(benchmarkInfo *benchmark) {
    unsigned long i;
    unsigned long count = benchmark->currentResultIndex;
    char timeTemp[100];
    for(i=0;i<count;i++){
        long long duration = benchmark->resultsToUse[i]->duration_miliseconds;
        sprintf(timeTemp,"%lld\n",duration);
        fputs(timeTemp,benchmark->flushFile);
    }
    fclose(benchmark->flushFile);
}

kvPair* getKvPair(benchmarkInfo* benchmark){
    int index = benchmark->currentKvPairIndex;
    int count = benchmark->count;
    if(index < count){
        benchmark->currentKvPairIndex +=1;
        return benchmark->kvPairToUse[index];
    }else{
        printf("We have used up all the kv pairs\n");    
        return NULL;
    }
}

void setFileName(benchmarkInfo *benchmark, char* name){
    sprintf(benchmark->name,"./data/d_%s",name);
    benchmark->flushFile = fopen(benchmark->name,"a");
    if(benchmark->flushFile == NULL){
        printf("unable to open file %s\n",benchmark->name);
    }
}
