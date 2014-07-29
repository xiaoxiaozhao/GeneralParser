#ifndef CONFIG_H
#define CONFIG_H

class Config
{
  public:
  Config(char* filename);
  virtual ~Config();

  // interface
  virtual bool parseConfig() = 0;

  protected:
  char* filename;
  FILE* file;
};

class SystemConfig
{
  public:
  SystemConfig(int argc, char** argv)
  {
    projectName = argv[1];
    batchSize = atoi(argv[2]);
    logChannel = argv[3];
    logLevel = (LOG_LEVEL)atoi(argv[4]);
  }
  ~SystemConfig()
  {
    if (logChannel)
      free(logChannel);
  }

  char* projectName;
  int batchSize;
  char* logChannel;
  LOG_LEVEL logLevel;
};

#endif
