#include<stdio.h>
#include"connect.h"
int main(){
  connectRedis();
/*  char* value = (char*)malloc(1024*8);
  set("key1","test1");
  set("key2","test2");
  set("key3","test3");
  set("key4","test4");


  get("key1",value);
  printf("get key1: %s\n",value);

  get("key2",value);
  printf("get key2: %s\n",value);

  get("key3",value);
  printf("get key3: %s\n",value);


  get("key4",value);
  printf("get key4: %s\n",value);
*/
  __clusterInfo(); 


  disconnectDatabase();
  return 0;
}
