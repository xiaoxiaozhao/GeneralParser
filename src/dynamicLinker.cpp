#include "../include/common.h"

bool ParserConfig::parseKv(char* line)
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

  if (!strcmp(key, "lib"))
  {
    if (lib)
      free(lib);
    lib = (char*)malloc(strlen(value));
    strcpy(lib, value);
  }
  else if (!strcmp(key, "method"))
  {
    if (method)
      free(method);
    method = (char*)malloc(strlen(value));
    strcpy(method, value);
  }
  else if (!strcmp(key, "preEncoding"))
  {
    if (!strcmp(value, "true"))
      preEncoding = true;
    else
      preEncoding = false;
  }
  else
    return false;

  return true;
}

DynamicLinker::DynamicLinker(RedisManager* redisMgr, ParserConfig* config, Logger* logger, DataHelper* helper)
{
  this->config = config;
  this->logger = logger;
  this->redisMgr = redisMgr;
  this->helper = helper;

  if (!loadParser())
  {
    exit(1);
  }

  return;
}

DynamicLinker::~DynamicLinker()
{
  unloadParser();
}

void DynamicLinker::invokeParser()
{
  (*parserFunc)(redisMgr, config, logger, helper);

  return;
}

bool DynamicLinker::loadParser()
{
  char* error;

  dlHandle = dlopen(config->lib, RTLD_LAZY);
  if (!dlHandle)
  {
    logger->error("GenernalParser::DynamicLinker::loadParser", dlerror());
    return false;
  }

  parserFunc = (void (*)(RedisManager*, ParserConfig*, Logger*, DataHelper*))dlsym(dlHandle, config->method);
  error = dlerror();
  if (error != NULL)
  {
    logger->error("GenernalParser::DynamicLinker::loadParser", error);
    return false;
  }

  return true;
}

bool DynamicLinker::unloadParser()
{
  dlclose(dlHandle);

  return true;
}
