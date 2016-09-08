#include<stdio.h>
#include<unistd.h>
#include"benchmarkHelp.h"

void test_init_and_load() {
    benchmarkInfo * benchmark = initBenchmark();     
    benchmark = loadData(benchmark);
    int count = benchmark->count;
    int i;
    kvPair *tempPair;
    for(i=0;i<count;i++) {
        long long start = timeStamp();
        tempPair = getKvPair(benchmark);
        printf("k=%s v=%s\n",tempPair->key,tempPair->value);
        sleep(1);
        long long end = timeStamp();
        addDuration(benchmark,end - start);        
    }
    for(i=0;i<count;i++){
        printf("duration = %lld\n",benchmark->resultsToUse[i]->duration_miliseconds);
    }

}


int main(){
   test_init_and_load();
   return 0;
}
