#include<stdio.h>
#include"benchmarkHelp.h"
void test_init_and_load(){
    benchmarkInfo * benchmark = initBenchmark();     
    benchmark = loadData(benchmark);
    int count = benchmark->count;
    int i;
    kvPair *tempPair;
    for(i=0;i<count;i++){
        tempPair = getKvPair(benchmark);
        printf("k=%s v=%s\n",tempPair->key,tempPair->value);
    }
}


int main(){
   test_init_and_load();
   return 0;
}
