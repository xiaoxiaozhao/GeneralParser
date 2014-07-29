#ifndef SIMPLE_HANDLER_1_H
#define SIMPLE_HANDLER_1_H

#define MAX_RESULT_LEN 8192

extern void copyString(char** dest, char* src);

class SimpleHandler1 : public Handler
{
  public:
  SimpleHandler1(DataHelper* helper, Logger* logger, RedisManager* redisMgr) : Handler(logger, redisMgr)
  {
    this->helper = helper;
  }
  ~SimpleHandler1();

  void freeString(char* str)
  {
    free(str);
  }

  char* spaceLink(char* str1, char* str2);
  char* getKV(char* key, char* value);

  DataHelper* helper;
};

#endif
