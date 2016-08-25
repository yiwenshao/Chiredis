#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>
#include "my_bench.h"
/*
"value" is a char* type global variable which is used to hold value.
It can be used in either get or set operation.
para1:
para2:
para3: thread id
*/
void *db_function(char* ip_in,int port_in,void* input){
  char * ip = ip_in;
  int port = port_in;
  printf("thread=%d\n",*(int*)input);
  int my_tid = *(int*)input;
  /*
  *each thread has its own value and cluster struct.
  */
  char*  value = (char*)malloc(1024*8); 
  char*  key = (char*)malloc(100);
  clusterInfo *cluster = connectRedis(ip,port);

  if(cluster!=NULL){
      printf("connected to cluster\n");
  }
  else{ 
  	printf("==null\n");
	exit(0);
  }

  int sum = 0;
 
  sprintf(key,"key=%d",*((int*)input));

  sprintf(value,"%s","aaaaa");

  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);


  sprintf(value,"%s","ffffff");
  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s:%s\n",key,value);


  sprintf(value,"%s","aefde");
  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);

  sprintf(value,"%s","abcdefads");
  sum += set(cluster,key,value,1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);
  
  if(sum == 0)
     printf("all operations succeed!\n");
  else printf("operation fail\n");

  //flushDb(cluster);
  disconnectDatabase(cluster);
  printf("intput = %d\n",*((int*)input)); 
}

/*
*try to write a small benchmark using timeval.
*this function is used in multi-threading environment.
*/
/*
void *db_bench(void* input){
  FILE * time_file;
 
  struct timeval tv1;
  struct timeval tv2;
  //record 20w operations for each thread
  long int * time_array = (long int*)malloc(450000*sizeof(long int));
  if (time_array == NULL){
      printf("time unallocated\n");
      return NULL;
  }

  char * ip = "192.168.2.11";
  int port = 7011;
  
  printf("thread=%d\n",*(int*)input);

  int my_tid = *(int*)input;
  char*  value = (char*)malloc(1024*8);

  char filename[20];
  sprintf(filename,"fileid=%d",my_tid);
  
  time_file = fopen(filename,"a+"); 

  clusterInfo *cluster = connectRedis(ip,port);
  assert(cluster !=NULL);
  if(cluster!=NULL){
  
  }
  else {
    printf("==null\n");
    return NULL;
  }
  char* key = (char*)malloc(100);
//start to get and set here
  int i=0;
  int count = 0;

  int why_up_count=0;
  int why_down_count=0;

  for(;i<200000;i++){
    sprintf(key,"%dkey=%d",my_tid,my_tid+i);
    gettimeofday(&tv1,NULL);
    sprintf(value,"time=%ld",tv1.tv_usec);
    get(cluster,key,value,1,my_tid);

    gettimeofday(&tv2,NULL);

    time_array[count] = tv2.tv_usec - tv1.tv_usec;
    if(time_array[count] <0 ){
        why_up_count++;
    }
    count++;

    printf("count=%d,id=%d\n",count,my_tid);

    if(strcmp(value,"nil")==0){
         gettimeofday(&tv1,NULL);
         int re = set(cluster,key,value,1,my_tid);
         gettimeofday(&tv2,NULL);
	 time_array[count] = tv2.tv_usec - tv1.tv_usec;
	 if(time_array[count]<0)
	    why_down_count++;
	 count++;
    }
  }
  printf("i=%d_______________________________________________________\n",i);
  time_array[count]=-1;  
  char temp_write[100];
  for(i=0;i<200000;i++){
      if(time_array[i]!=-1){
          sprintf(temp_write,"%ld\n",time_array[i]);
          fputs(temp_write,time_file);
      }    
      else break;
  }
  sprintf(temp_write,"why_up=%d, why_down=%d \n",why_up_count,why_down_count);
  fputs(temp_write,time_file);

  flushDb(cluster);
  disconnectDatabase(cluster);

}
*/
/*
void test_with_multiple_threads(){
 pthread_t th[16];
 int res, i;
 init_global();
 void* thread_result;
 int thread[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
 for(i=0;i<16;i++){ 
   res = pthread_create(&th[i],NULL,db_bench,(void*)(&thread[i]));
   if(res!=0){
      printf("thread fail\n");
      break;
   }
 }
 for(i=0;i<16;i++){ 
   res = pthread_join(th[i],&thread_result);
   if(res!=0){
      printf("thread join fail\n");
      break;
     }
  }
}
*/

//********************************************************
void single_pipe_bench_set(char* filename,int num){
     int port=6379;
     char* ip="127.0.0.1";
     FILE* fp;
     singleClient* sc = single_connect(port,ip); 

     if(fp=freopen(filename,"r",stdin)==NULL){
         fprintf(stderr,"error redirection\n");
     }
     char* key=(char*)malloc(500);
     char* value=(char*)malloc(500);
     int i;
     int pipe_len=16;
     time_t now,then;
     now = time(0);
     while(num>0){
          for(i=0;i<pipe_len;i++){
             scanf("%s %s",key,value);
             pipe_set(sc,key,value);
         }
         pipe_getAllReply(sc);
	 num-=pipe_len;
     }
     then = time(0);
     printf("time taken: %lld\n",then - now);
     single_disconnect(sc);
     //fclose(fp); no need.
}


int main(){
 //init_global();
 pipe_example("now");

 return 0;
}
