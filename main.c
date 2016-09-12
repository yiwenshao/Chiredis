#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>
#include "my_bench.h"

int main(){
//init_global();
//  pipe_example("192.168.1.21",6379,"now");
    test_with_multiple_threads("192.168.1.22",6667);
//    test_pipeline_with_multiple_threads("192.168.1.22",6667);
    return 0;
}

