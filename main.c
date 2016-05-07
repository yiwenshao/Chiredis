#include<stdio.h>
#include"connect.h"
/*
*"value" is a char* type global variable which is used to hold value.
*It can be used in either get or set operation.
*/

int main(){
  char * ip = "115.29.113.239";
  int port = 7002;
  char*  value = (char*)malloc(1024*8); 
  clusterInfo *cluster = connectRedis(ip,port);
  if(cluster!=NULL)
  	printf("!=null\n");
  else printf("==null\n");
//  print_clusterInfo_parsed(cluster);

  int sum = 0;
  sum += set(cluster,"db1a","rr",1);
  sum += set(cluster,"db2a","rr",1);
  sum += set(cluster,"db3a","xr",1);
  sum += set(cluster,"db4a","xr",1);
  
  sum += get("db1a",value,1);
  printf("get db1: %s\n",value);

  sum += get("db2a",value,1);
  printf("get db2: %s\n",value);

  sum += get("db3a",value,1);
  printf("get db3: %s\n",value);

  sum += get("db4a",value,1);
  printf("get db4: %s\n",value);
  
  if(sum == 0)
     printf("all operations succeed!\n");
  else printf("operation fail\n");


  flushDb();
  disconnectDatabase();
  return 0;
}
