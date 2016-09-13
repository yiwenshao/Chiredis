#include<stdio.h>
#include<unistd.h>
#include"benchmarkHelp.h"

void test_init_and_load() {
    benchmarkInfo * benchmark = initBenchmark(5000000);     
    benchmark = loadData(benchmark);
    int count = benchmark->count;
    int i;
    kvPair *tempPair;
    for(i=0;i<count;i++) {
        long long start = us_time();
        tempPair = getKvPair(benchmark);
        printf("k=%s v=%s\n",tempPair->key,tempPair->value);
        sleep(1);
        long long end = us_time();
        addDuration(benchmark,end - start);        
    }
    char temp[11] = "123";
    setFileName(benchmark,temp);
    for(i=0;i<count;i++){
        printf("duration = %lld\n",benchmark->resultsToUse[i]->duration_miliseconds);
    }
    flushResults(benchmark);


}

void test_config(){
    benchmarkConfig * bc = init_config();
    show_config(bc);    


}
int main(){
   test_init_and_load();
   // test_config();
    return 0;
}
