#include "../include/common.h"

Config::Config(char* filename)
{
  this->filename = filename;

  file = fopen(filename, "r");
  if (file == NULL)
  {
    exit(1);
  }
}

Config::~Config()
{
  if (file != NULL)
    fclose(file);
}

