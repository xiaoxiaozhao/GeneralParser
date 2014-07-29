#ifndef DATA_HELPER_H
#define DATA_HELPER_H

typedef struct _DataPacket
{
  char* payload;
  bool success;
} DataPacket;

class DataHelper
{
  public:
  DataHelper(RedisManager* redisMgr, ParserConfig* parserConfig, Logger* logger);
  ~DataHelper();

  char* getStr(char* data);
  void assemble(char* str, char* err, bool complete);
  
  RedisManager* redisMgr;
  ParserConfig* parserConfig;
  Logger* logger;

  char* escapeStr(char* src);
  char* data;
};

#endif
