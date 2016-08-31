#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include <errno.h>

void __clusterInfo(){
    char *argv[20];
    redisContext* c = redisConnect("192.168.0.5", 6687);
    if(c == NULL||c->err){
       printf("unable to connection\n");
       return ;
    }
    redisReply* r = (redisReply*)redisCommand(c,"cluster nodes");
	printf("%s\n",r->str);
    char* temp = r->str;
    char * point;       
    printf("%s\n",strstr(temp,"connected"));

}


int main(){
	__clusterInfo();
	return 0;
}
