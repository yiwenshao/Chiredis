#include<stdio.h>
#include"connect.h"
int main(){
  connectRedis("115.29.113.239",7002);


  set("db1","dalfdkjslfdjslfdjsal",1);
  set("db2","dalfdkjslfdjslfdjsal",1);
  set("db3","dalfdkjslfdjslfdjsal",1);
  set("db4","dalfdkjslfdjslfdjsal",1);
  
  get("db1",value,1);
  printf("get mycluster4: %s\n",value);

  get("db2",value,1);
  printf("get mycluster4: %s\n",value);

  get("db3",value,1);
  printf("get mycluster4: %s\n",value);

  get("db4",value,1);
  printf("get mycluster4: %s\n",value);
  flushDb();

  disconnectDatabase();
  return 0;
}
