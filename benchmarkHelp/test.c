#include<stdio.h>
#include"benchmarkHelp.h"
void test_init_and_load(){
    benchmarkInfo * benchmark = initBenchmark();     
    benchmark = loadData(benchmark);
    int count = benchmark->count;
    int i;
    for(i=0;i<count;i++){
        printf("k=%s v=%s\n",benchmark->kvPairToUse[i]->key,benchmark->kvPairToUse[i]->value);
    }
     
}


int main(){
   test_init_and_load();
   return 0;
}
