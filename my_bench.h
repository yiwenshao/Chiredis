#ifndef MY_BENCH_H
#define MY_BENCH_H
typedef struct kvload{
    char** rkey;
    char** rvalue;
    char** wkey;
    char** wvalue;
    char* readFile;
    char* writeFile;
}
#endif
