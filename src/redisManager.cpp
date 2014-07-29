#include "../include/common.h"

extern SystemConfig* sysConf;

RedisConfig::~RedisConfig()
{
  if (host)
    free(host);
  if (dataInputChannel)
    free(dataInputChannel);
  if (dataOutputChannel)
    free(dataOutputChannel);
  if (dataBypassChannel)
    free(dataBypassChannel);
}

bool RedisConfig::parseConfig()
{
  char line[256];

  while (fgets(line, 256, file) != NULL)
  {
    parseKv(line);
  }

  return true;
}

bool RedisConfig::parseKv(char* line)
{
  char* key;
  char* value;
  
  // this is dirty...
  if (strrchr(line, '\n'))
    *(strrchr(line, '\n')) = '\0';
  char deli = '=';
  // get key
  key = line;
  // get value
  value = (char*)(strchr(line, deli) + 1);
  *(strchr(line, deli)) =  '\0';
  // remove spaces from key and value
  if (strrchr(key, ' '))
    *(strchr(key, ' ')) = '\0';
  if (strrchr(value, ' '))
    value = strrchr(value, ' ');

  if (!strcmp(key, "host"))
  {
    if (host)
      free(host);
    host = (char*)malloc(strlen(value));
    strcpy(host, value);
  }
  else if (!strcmp(key, "port"))
  {
    port = atoi(value);
  }
  else if (!strcmp(key, "dataInputChannel"))
  {
    if (dataInputChannel)
      free(dataInputChannel);
    dataInputChannel = (char*)malloc(strlen(value));
    strcpy(dataInputChannel, value);
  }
  else if (!strcmp(key, "dataOutputChannel"))
  {
    if (dataOutputChannel)
      free(dataOutputChannel);
    dataOutputChannel = (char*)malloc(strlen(value));
    strcpy(dataOutputChannel, value);
  }
  else if (!strcmp(key, "dataBypassChannel"))
  {
    if (dataBypassChannel)
      free(dataBypassChannel);
    dataBypassChannel = (char*)malloc(strlen(value));
    strcpy(dataBypassChannel, value);
  } 
  else
    return false;

  return true;
}

RedisManager::RedisManager(RedisConfig* config, SystemConfig* sysConf)
{
  this->sysConf = sysConf;
  this->config = config;

  if (!connect())
    exit(1);

  if (!startPublishThread())
    exit(1);

  if (!startSubscribeThread())
    exit(1);
}

RedisManager::~RedisManager()
{
  pthread_mutex_destroy(&pubMutex);
  pthread_cond_destroy(&pubCond);

  pthread_mutex_destroy(&subMutex);
  pthread_cond_destroy(&subCond);

  disconnect();
}

bool RedisManager::connect()
{
  pubContext = redisConnect(config->host, config->port);
  if (pubContext != NULL && pubContext->err)
  {
    printf("%d\n", pubContext->err);
    return false;
  }

  subContext = redisConnect(config->host, config->port);
  if (subContext != NULL && subContext->err)
  {
    printf("%d\n", subContext->err);
    return false;
  }

  successContext = redisConnect(config->host, config->port);
  if(successContext != NULL && successContext->err)
  {
	  printf("%d\n", successContext->err);
	  return false;
  }
  redisCommand(successContext, "set successCount 0");

  totalContext = redisConnect(config->host, config->port);
  if(totalContext != NULL && totalContext->err)
  {
	  printf("%d\n", totalContext->err);
	  return false;
  }
  redisCommand(totalContext, "set totalCount 0");
  return true;
}

bool RedisManager::disconnect()
{
  redisFree(pubContext);
  redisFree(subContext);

  return true;
}

bool RedisManager::startPublishThread()
{
  int err;
  char error[1024];

  // init mutex & cond
  pthread_mutex_init(&pubMutex, NULL);
  pthread_cond_init(&pubCond, NULL);

  // create thread
  err = pthread_create(&publishThread, NULL, publishThreadFunc, this);
  if (err != 0)
  {
    return false;
  }

  // detach child thread
  pthread_detach(publishThread);

  return true;
}

bool RedisManager::startSubscribeThread()
{
  int err;
  char error[1024];

  // init mutex & cond
  pthread_mutex_init(&subMutex, NULL);
  pthread_cond_init(&subCond, NULL);

  // create thread
  err = pthread_create(&subscribeThread, NULL, subscribeThreadFunc, this);
  if (err != 0)
  {
    return false;
  }

  // detach child thread
  pthread_detach(subscribeThread);

  return true;
}

char* RedisManager::getData()
{
  char* ret = NULL;

  pthread_mutex_lock(&subMutex);
  if (subList.size() == 0)
  {
    // no data for now, wait
    pthread_cond_wait(&subCond, &subMutex);
    if (subList.size() != 0)
    {
      ret = subList.front();
      subList.pop_front();
    }
    pthread_mutex_unlock(&subMutex);
  }
  else
  {
    // there's data, go ahead
    ret = subList.front();
    subList.pop_front();
    pthread_mutex_unlock(&subMutex);
  }

  return ret;
}

void RedisManager::setData(DataPacket* data)
{
  pthread_mutex_lock(&pubMutex);

  pubList.push_back(data);
  redisCommand(successContext, "INCR successCount");
  pthread_cond_signal(&pubCond);
  pthread_mutex_unlock(&pubMutex);
}

void* publishThreadFunc(void* args)
{
  RedisManager* redisMgr = (RedisManager*)args;
  redisContext* c = redisMgr->pubContext;
  redisReply* r;
  pthread_mutex_t* pubMutex = &(redisMgr->pubMutex);
  pthread_cond_t* pubCond = &(redisMgr->pubCond);
  list<DataPacket*>* pubList = &(redisMgr->pubList);

  DataPacket* data;
  int pktCount = 0;

  while (true)
  {
    pthread_mutex_lock(pubMutex);
    if (pubList->size() == 0)
    {
      // no data for now, wait
      pthread_cond_wait(pubCond, pubMutex);
      if (pubList->size() != 0)
      {
        data = pubList->front();
        pubList->pop_front();
        pthread_mutex_unlock(pubMutex);

        if (data->success)
          redisAppendCommand(c, "PUBLISH %s %s", redisMgr->config->dataBypassChannel, data->payload);
        else
          redisAppendCommand(c, "PUBLISH %s %s", redisMgr->config->dataOutputChannel, data->payload);
      }
    }
    else
    {
      // there's data, go ahead
      data = pubList->front();
      pubList->pop_front();
      pthread_mutex_unlock(pubMutex);

      if (data->success)
        redisAppendCommand(c, "PUBLISH %s %s", redisMgr->config->dataBypassChannel, data->payload);
      else
        redisAppendCommand(c, "PUBLISH %s %s", redisMgr->config->dataOutputChannel, data->payload);
    }

    // clear memory
    //free(data->payload);
    //free(data);

    // Pipelining
    pktCount++;
    if (pktCount >= sysConf->batchSize)
    {
      for (int i=0; i<pktCount; i++)
      {
        redisGetReply(c, (void**)&r);
        freeReplyObject(r);
      }

      pktCount = 0;
    }
  }

  // TO DO : exit pub thread safely

  return NULL;
}

void* subscribeThreadFunc(void* args)
{
  RedisManager* redisMgr = (RedisManager*)args;
  redisContext* c = redisMgr->subContext;

  // subscribe channel
  redisReply* r;
  r = (redisReply*)redisCommand(c, "SUBSCRIBE %s", redisMgr->config->dataInputChannel);
  freeReplyObject(r);

  // get every possible reply
  while (redisGetReply(c, (void**)&r) == REDIS_OK)
  {
    if (r->type == REDIS_REPLY_ARRAY && r->elements == 3)
    {
      // read from redis
      int replyLen = strlen(r->element[2]->str);
      char* str = (char*)malloc(replyLen + 1);
      str[replyLen] = '\0';
      memcpy(str, r->element[2]->str, replyLen);

      printf("GET: %s\n", str);

      // save to subscribe list
      pthread_mutex_lock(&(redisMgr->subMutex));
      redisMgr->subList.push_back(str);
	  redisCommand(redisMgr->totalContext, "INCR totalCount");
	  freeReplyObject(r);
      pthread_cond_signal(&(redisMgr->subCond));
      pthread_mutex_unlock(&(redisMgr->subMutex));
    }
    else
      freeReplyObject(r);
  }

  // TO DO : exit sub thread safely

  return NULL;
}
