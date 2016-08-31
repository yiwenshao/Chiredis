#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include <errno.h>

int main(){
    redisContext *c = redisConnect("127.0.0.1",6379); 
    if (c == NULL || c->err){
        printf("error1\n");
    } else {
        printf("succeed\n");
    }

    redisAppendCommand(c,"set %s %s","k1","v1");
    redisAppendCommand(c,"set %s %s","k2","v2");
    redisAppendCommand(c,"set %s %s","k3","v3");
    redisAppendCommand(c,"set %s %s","k4","v4");
    redisAppendCommand(c,"set %s %s","k5","v5");
    redisAppendCommand(c,"set %s %s","k6","v6");
    redisAppendCommand(c,"set %s %s","k7","v7");
    redisAppendCommand(c,"set %s %s","k8","v8");
    redisAppendCommand(c,"set %s %s","k9","v9");
    redisAppendCommand(c,"set %s %s","k0","v0");
    redisReply *reply;

    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
        printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
        printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);
    redisGetReply(c,(void**)&reply);
    printf("%s\n",reply->str);



   return 0;
}
