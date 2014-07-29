#include "/usr/local/include/hiredis/hiredis.h"
#include <stdio.h>
#include <unistd.h>
#include "time.h"
#define LINE_LEN 1024
char line[LINE_LEN];

int main()
{
  FILE* file = fopen("atm.log", "r");
  clock_t start,finish;
  double d;
  if (file == NULL)
  {
    printf("Cannot open file\n");
    return 1;
  }

  redisContext* c = redisConnect("127.0.0.1", 6379);
  redisReply* reply;
  start=clock();	
  while (!feof(file))
  {
    fgets(line, LINE_LEN, file);
    reply = (redisReply*)redisCommand(c, "PUBLISH %s %s", "zlb1", line);
    freeReplyObject(reply);
    //sleep(1);
  }
  finish=clock();
  d=(double)(finish-start)/CLOCKS_PER_SEC;
  printf("time:%f\n",d);
  fclose(file);
  return 0;
}
