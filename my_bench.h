#ifndef MY_BENCH_H
#define MY_BENCH_H
typedef struct benchRecord{
    unsigned long number;
    char** rkey;
    char** rvalue;
    char** wkey;
    char** wvalue;
    char* readFile;
    char* writeFile;
}benchRecord;

benchRecord* loadFile(char* rfileName,char* wfileName,unsigned long num);



#endif
