#include "../include/common.h"

DataHelper::DataHelper(RedisManager* redisMgr, ParserConfig* parserConfig, Logger* logger)
{
  this->redisMgr = redisMgr;
  this->parserConfig = parserConfig;
  this->logger = logger;
}

DataHelper::~DataHelper()
{
  if (data)
    free(data);
}

char* DataHelper::getStr(char* data)
{
  this->data = data;
  char* str;

  char* pos = strchr(data, ' ');
  if (pos != NULL)
  {
    int len = (int)(pos - data);
    str = (char*)malloc(len + 2);
    str[len + 1] = '\0';
    memcpy(str, data, len + 1);
  }
  else // this is the first stage
  {
    int len = strlen(data);
    if (parserConfig->preEncoding) // so only first stage can have this flag
    {
      str = (char*)malloc(16384);
      int gbLen = u2g(data, len, str, 16384);
      if (!gbLen)
      {
        char error[20480];
        sprintf(error, "g2u failed - %s", str);
        logger->error("Datahelp::getStr", error);
        free(str);
        return NULL;
      }
    }
  }

  return escapeStr(str);
}

void DataHelper::assemble(char* str, char* err, bool complete)
{
  DataPacket* pkt = (DataPacket*)malloc(sizeof(DataPacket));

  if (complete)
  {
    pkt->payload = str;
    pkt->success = true;

    redisMgr->setData(pkt);
  }
  else
  {
    int len = strlen(str) + strlen(err);
    char* output = (char*)malloc(len + 2);
    output[0] = '\0';
    strcat(output, data);
    strcat(output, str);

    pkt->payload = output;
    pkt->success = false;

    redisMgr->setData(pkt);
  }
}

char* DataHelper::escapeStr(char* src)
{
  char esc[] = " =";
  char tgt[] = "@^";
  int len = strlen(esc);

  for (unsigned int i=0; i<len; i++)
  {
    char* str = strchr(src, esc[i]);
    while (str != NULL)
    {
      *str = tgt[i];
      str = strchr(str+1, esc[i]);
    }
  }

  return src;
}
