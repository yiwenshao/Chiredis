#include<stdio.h>
#include"connect.h"
#include<pthread.h>
#include<time.h>

/*
*"value" is a char* type global variable which is used to hold value.
*It can be used in either get or set operation.
*/
void *db_function(void* input){
  char * ip = "192.168.2.11";
  int port = 7011;
  printf("thread=%d\n",*(int*)input);
  int my_tid = *(int*)input;
  /*
  *each thread has its own value and cluster struct.
  */
  char*  value = (char*)malloc(1024*8); 
  clusterInfo *cluster = connectRedis(ip,port);
  if(cluster!=NULL){
  	//printf("!=null\n");
  }
  else printf("==null\n");

  int sum = 0;
  char* key = (char*)malloc(100);

  sprintf(key,"key=%d",*((int*)input));

  sum += set(cluster,key,"aaaaa",1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);

  sum += set(cluster,key,"fffff",1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s:%s\n",key,value);

  sum += set(cluster,key,"eafde",1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);

  sum += set(cluster,key,"abcde",1,my_tid);
  sum += get(cluster,key,value,1,my_tid);
  printf("get %s: %s\n",key,value);
  
  if(sum == 0)
     printf("all operations succeed!\n");
  else printf("operation fail\n");

  flushDb(cluster);
  disconnectDatabase(cluster);
  printf("intput = %d\n",*((int*)input)); 
}

/*
*try to write a small benchmark using timeval.
*/
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

int main(){
 init_global();
// test_with_multiple_threads();

  char * ip = "115.29.113.239";
  int port = 5674;
  clusterInfo *cluster = connectRedis(ip,port);
  if(cluster == NULL){
     printf("unable to connect\n");
     return 0;
  }
  
  char* key = (char*)malloc(100);
  char* value = (char*)malloc(100);
  sprintf(key,"%s","mykeynow");
  sprintf(value,"%s","myvaluenow");

  set(cluster,key,value,0,0);
  int g=get(cluster,key,value,0,0);

  printf("get result=%d, value=%s\n",g,value);

  flushDb(cluster);
  disconnectDatabase(cluster);


 return 0;
}
