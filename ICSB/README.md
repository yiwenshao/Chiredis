This module tries to add a general framework for benchmarking database systems. It can be used as a 
minimalistic k-v benchmark engine.
It provides some help functions for testing.
1.load
    load kv pairs into memory
2.start/end 
    to test time duration
3.put
    to put the duration into an array
4.flush
    flush data into disk for furthur analysis

How to use ?
1. mordify token.c
2. make target source=token.c depend="-lchiredis -lhiredis"
3. enjoy you benchmark



### Use tinybenchmark for redis

Modify benchmarkConfig/benchmark.config

+ keyLen: 
+ valueLen: 
+ totalCount: number of kv pairs for each thread
+ threadCount: number of threads




```
// start a pipeline test with default pipecount 20
./tinyBenchmark ip port -s pipeline

//start a normal test
./tinyBenchmark ip port -s normal

```

