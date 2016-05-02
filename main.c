#include<stdio.h>
#include"connect.h"
int main(){
  connectRedis("172.16.30.211",7002);
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
  clusterInfo* cif =   __clusterInfo(); 
  assign_slot(cif);
  __test_slot(cif);


  disconnectDatabase();
  return 0;
}
