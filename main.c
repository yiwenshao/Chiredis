#include<stdio.h>
#include"connect.h"
int main(){
  connectRedis("172.16.30.211",7002);

  char* value = (char*)malloc(1024*8);

  set("mycluster1","dalfdkjslfdjslfdjsal");
  set("mycluster2","dalfdkjslfdjslfdjsal");
  set("mycluster3","dalfdkjslfdjslfdjsal");
  set("mycluster4","dalfdkjslfdjslfdjsal");
  
  get("mycluster4",value);
  printf("get mycluster4: %s\n",value);

  get("mycluster4",value);
  printf("get mycluster4: %s\n",value);

  get("mycluster4",value);
  printf("get mycluster4: %s\n",value);


  get("mycluster4",value);
  printf("get mycluster4: %s\n",value);


  disconnectDatabase();
  return 0;
}
