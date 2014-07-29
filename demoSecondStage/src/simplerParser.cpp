#include "../../include/common.h"
#include "../include/handler.h"
#include "../include/simplerHandler1.h"

#define MAX_RES_LEN 10240

bool simplerParse(SimplerHandler1* handler, list<char*>* keyList, char* line, char* result);

extern "C" void parserFunc(RedisManager* mgr, ParserConfig* config, Logger* log, DataHelper* help)
{
  // init
  char* line = NULL;
  DataPacket* pkt = NULL;
  list<char*>* keyList = new list<char*>();
  // init key list
  keyList->push_back("start");
  keyList->push_back("terminal");
  keyList->push_back("mainBrc");
  keyList->push_back("subBrc");
  keyList->push_back("teller");
  keyList->push_back("operation");
  keyList->push_back("account");
  keyList->push_back("amount");
  keyList->push_back("end");
  keyList->push_back("hostStat");
  keyList->push_back("termStat");
  keyList->push_back("termCode");
  keyList->push_back("codeDesc");
  keyList->push_back("termSerial");
  keyList->push_back("hostSerial");
  keyList->push_back("remark");

  SimplerHandler1* handler = new SimplerHandler1(help, log, mgr);

  while(true)
  {
    // read from redis
    line = mgr->getData();
    printf("%s\n", line);
    // we don't have to free pkt since publish thread will do that
    pkt = (DataPacket*)malloc(sizeof(DataPacket));
    pkt->payload = (char*)malloc(MAX_RES_LEN);

    // parse
    pkt->success = simplerParse(handler, keyList, line, pkt->payload);
    
    // write to redis
    mgr->setData(pkt);

    // clean memory
    free(line);

    // TO DO : exit parser safely
  }

  return;
}

bool simplerParse(SimplerHandler1* handler, list<char*>* keyList, char* line, char* result)
{
  // just split line

  list<char*>::iterator iter = keyList->begin();
  result[0] = '\0';
  int keyIndex = 0;
  char* valueStart;

  valueStart = strtok(line, "\t");
  while (valueStart != NULL)
  {
    if (strlen(valueStart) != 0)
    {
      // push new pair to kv string
      if (keyIndex != 0)
        strcat(result, " ");

      strcat(result, *iter);
      strcat(result, "=");
      strcat(result, valueStart);
    }

    valueStart = strtok(NULL, "\t");
    iter++;
    keyIndex++;
  }

  printf("%s\n", result);
  return true;
}
