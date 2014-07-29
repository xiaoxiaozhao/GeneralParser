#include "../include/common.h"

Logger::Logger(RedisConfig* redisConfig, SystemConfig* sysConf)
{
  this->sysConf = sysConf;
  this->redisConfig = redisConfig;

  this->logLevel = sysConf->logLevel;
  logChannel = sysConf->logChannel; 
  context = redisConnect(redisConfig->host, redisConfig->port);
  if (context != NULL && context->err)
  {
    printf("%d\n", context->err);
    exit(1);
  }
}

Logger::~Logger()
{
  // do nothing
}

void Logger::error(char* module, char* msg)
{
  printLog(ERROR, module, msg);
}

void Logger::warning(char* module, char* msg)
{
  printLog(WARNING, module, msg);
}

void Logger::debug(char* module, char* msg)
{
  printLog(DEBUG, module, msg);
}

void Logger::log(char* module, char* msg)
{
  printLog(LOG, module, msg);
}

void Logger::printLog(LOG_LEVEL level, char* module, char* msg)
{
  if (level < logLevel)
    return;

  char* timestamp = getTimestamp();
  char log[MAX_LOG_LEN];
  log[0] = '\0';

  strcat(log, timestamp);
  strcat(log, "|");
  strcat(log, module);
  strcat(log, "|");
  strcat(log, msg);

  publishLog((char*)log);

  free(timestamp);
}

bool Logger::publishLog(char* data)
{
  redisReply* reply;

  reply = (redisReply*)redisCommand(context, "PUBLISH %s %s", logChannel, data);

  freeReplyObject(reply);

  return true;
}

char* Logger::getTimestamp()
{
  char* timestamp = (char*)malloc(80);
  time_t rawTime;
  struct tm* now;

  time(&rawTime);
  now = localtime(&rawTime);
  strftime(timestamp, 80, "%F %T", now);

  return timestamp;
}
