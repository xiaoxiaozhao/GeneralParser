#ifndef DYNAMIC_LINKER_H
#define DYNAMIC_LINKER_H

class ParserConfig : public Config
{
  public:
  ParserConfig(char* filename) : Config(filename)
  {
    lib = NULL;
    method = NULL;
    preEncoding = false;

    parseConfig();
  }

  ~ParserConfig()
  {
    if (lib)
      free(lib);
    if (method)
      free(method);
  }

  bool parseConfig()
  {
    char line[256];

    while (fgets(line, 256, file) != NULL)
      parseKv(line);
  }
  bool parseKv(char* line);

  char* lib;
  char* method;
  bool preEncoding;
};

class DynamicLinker
{
  public:
  DynamicLinker(RedisManager* redisMgr, ParserConfig* config, Logger* logger, DataHelper* helper);
  ~DynamicLinker();

  // interface
  void invokeParser();

  private:
  // data member
  void* dlHandle;
  void (*parserFunc)(RedisManager*, ParserConfig* config, Logger* logger, DataHelper* helper);

  ParserConfig* config;
  Logger* logger;
  RedisManager* redisMgr;
  DataHelper* helper;
  
  // helper
  bool loadParser();
  bool unloadParser();
};

#endif
