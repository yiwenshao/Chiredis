#ifndef MY_BENCH_H
#define MY_BENCH_H
typedef struct thread_struct{
  char* in_ip;
  int in_port;
  int tid;
}thread_struct;

void pipe_example(char* filename);

void test_with_multiple_threads(char*ip,int port);
#endif
