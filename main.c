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
  connectRedis(ip,port);

  int sum = 0;
  sum += set("db1a","djxxsalrrr",1);
  sum += set("db2a","slfdxxjsalrrr",1);
  sum += set("db3a","slfdjsxxalrrr",1);
  sum += set("db4a","jslfdjsaxxlrrr",1);
  
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
