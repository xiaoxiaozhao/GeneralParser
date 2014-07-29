#ifndef LOGGER_H
#define LOGGER_H

#define MAX_LOG_LEN 1024
#define LOG_CHANNEL GeneralParser::Log

class RedisManager;
class RedisConfig;
class SystemConfig;

class Logger
{
  public:
  Logger(RedisConfig* redisConfig, SystemConfig* sysConf);
  ~Logger();
  void error(char* module, char* msg);
  void warning(char* module, char* msg);
  void debug(char* module, char* msg);
  void log(char* module, char* msg);

  private:
  SystemConfig* sysConf;
  redisContext* context;
  RedisConfig* redisConfig;
  LOG_LEVEL logLevel;
  char* logChannel;

  void printLog(LOG_LEVEL level, char* module, char* msg);
  bool publishLog(char* data);
  char* getTimestamp();
};

#endif
