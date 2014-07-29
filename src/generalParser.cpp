#include "../include/common.h"

SystemConfig* sysConf;

int main(int argc, char** argv)
{
  // read parameters
  sysConf = new SystemConfig(argc, argv);

  // read config files
  char redisConfigPath[1024];
  strcpy(redisConfigPath, sysConf->projectName);
  strcat(redisConfigPath, "Redis.cfg");
  char parserConfigPath[1024];
  strcpy(parserConfigPath, sysConf->projectName);
  strcat(parserConfigPath, "Parser.cfg");

  RedisConfig* redisConfig = new RedisConfig(redisConfigPath);
  ParserConfig* parserConfig = new ParserConfig(parserConfigPath);
  
  // init vars
  RedisManager* redisMgr = new RedisManager(redisConfig, sysConf);
  Logger* logger = new Logger(redisConfig, sysConf);
  redisMgr->logger = logger; // we should decouple this
  DataHelper* helper = new DataHelper(redisMgr, parserConfig, logger);
  DynamicLinker* linker = new DynamicLinker(redisMgr, parserConfig, logger, helper);
  
  // run parser
  linker->invokeParser();

  return 0;
}
