#ifndef BENCHMARK_HELP
#define BENCHMARK_HELP
char* benchmarkHelpVersion();

typedef struct kvPair{
    char* key;
    char* value;
}kvPair;

typedef struct results{
    long long duration_miliseconds;
}results;

typedef struct benchmarkInfo{
    //name of the benchmark
    char name[50];
    //thread id, intended for multi-threading uses
    unsigned short tid;
    //key length and value length
    unsigned int keyLen;
    unsigned int valueLen;
    //an array of kv Pairs
    kvPair** kvPairToUse;
    //an array of results
    results** resultsToUse;
    //total number of kvs
    unsigned long count;
    //current index of kvPairToUse, use it to fetch a pointer to kvPair
    unsigned long currentKvPairIndex;
    unsigned long currentResultIndex;
}benchmarkInfo;

benchmarkInfo* initBenchmark();
benchmarkInfo* loadData(benchmarkInfo* info);
long long timeStamp();

void addDuration();

void freeBenchmark();

void flushResults();

kvPair* getKvPair(benchmarkInfo* benchmark);

#endif
