#ifndef REDIS_MANAGER_H
#define REDIS_MANAGER_H

class Logger;

class RedisConfig : public Config
{
  public:
  RedisConfig(char* filename) : Config(filename)
  {
    host = NULL;
    port = 0;
    dataInputChannel = NULL;
    dataOutputChannel = NULL;

    parseConfig();
  }
  ~RedisConfig();

  bool parseConfig();
  bool parseKv(char* line);

  // this is a data holder so data members are public
  char* host;
  int port;
  // be careful, input means 'put into', output means 'get from'
  char* dataInputChannel;
  char* dataOutputChannel;
  // well, bypass means 'directly send to backend'
  char* dataBypassChannel;

  private:
};

class RedisManager
{
  public:
  RedisManager(RedisConfig* config, SystemConfig* sysConf);
  ~RedisManager();

  bool startPublishThread();
  bool startSubscribeThread();

  char* getData();
  void setData(DataPacket* data);

  SystemConfig* sysConf;

  RedisConfig* config;
  redisContext* pubContext;
  redisContext* subContext;
  redisContext* successContext;
  redisContext* totalContext;
  Logger* logger;

  list<DataPacket*> pubList;
  list<char*> subList;

  pthread_t publishThread;
  pthread_mutex_t pubMutex;
  pthread_cond_t pubCond;

  pthread_t subscribeThread;
  pthread_mutex_t subMutex;
  pthread_cond_t subCond;
  

  bool connect();
  bool disconnect();
};

void* publishThreadFunc(void* args);
void* subscribeThreadFunc(void* args);

#endif
