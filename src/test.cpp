#include "/usr/local/include/hiredis/hiredis.h"
#include <stdio.h>

int main()
{
  redisContext* context = redisConnect("127.0.0.1", 6379);
  redisReply* reply = (redisReply*)redisCommand(context, "SUBSCRIBE output");
  freeReplyObject(reply);
  while (redisGetReply(context, (void**)&reply) == REDIS_OK)
  {
    if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 )
      printf("Got: %s\n", reply->element[2]->str);

    freeReplyObject(reply);
  }
}
