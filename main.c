#include<stdio.h>
#include"connect.h"
/*
*"value" is a char* type global variable which is used to hold value.
*It can be used in either get or set operation.
*/

int main(){
  char * ip = "115.29.113.239";
  int port = 7002;
  /*
  *each thread has its own value and cluster struct.
  */
  char*  value = (char*)malloc(1024*8); 
  clusterInfo *cluster = connectRedis(ip,port);
  if(cluster!=NULL)
  	printf("!=null\n");
  else printf("==null\n");

  int sum = 0;
  sum += set(cluster,"db1a","rXXXr",1);
  sum += set(cluster,"db2a","rXXXr",1);
  sum += set(cluster,"db3a","xXXXr",1);
  sum += set(cluster,"db4a","xXXXr",1);
  
  sum += get(cluster,"db1a",value,1);
  printf("get db1: %s\n",value);

  sum += get(cluster,"db2a",value,1);
  printf("get db2: %s\n",value);

  sum += get(cluster,"db3a",value,1);
  printf("get db3: %s\n",value);

  sum += get(cluster,"db4a",value,1);
  printf("get db4: %s\n",value);
  
  if(sum == 0)
     printf("all operations succeed!\n");
  else printf("operation fail\n");


  flushDb(cluster);
  disconnectDatabase(cluster);
  return 0;
}
