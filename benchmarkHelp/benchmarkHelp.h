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
    //total number of kvs
    unsigned long count;
    unsigned int keyLen;
    unsigned int valueLen;
    kvPair** kvPairToUse;
    results** resultsToUse;
}benchmarkInfo;

benchmarkInfo* initBenchmark();
benchmarkInfo* loadData(benchmarkInfo* info);
void timeStamp();

void addDuration();

void freeBenchmark();

void flushResults();

kvPair* getKvPair(benchmarkInfo* benchmark);

#endif
