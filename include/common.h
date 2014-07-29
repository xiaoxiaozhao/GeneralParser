#ifndef COMMON_H
#define COMMON_H

enum LOG_LEVEL
{
  DEBUG,
  WARNING,
  ERROR,
  LOG
};

class RedisManager;
class RedisConfig;
class SystemConfig;
class Logger;
class Config;
class ParserConfig;
class DynamicLinker;
class DataHelper;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
using namespace std;
#include <pthread.h>
#include "/usr/local/include/hiredis/hiredis.h"
#include <dlfcn.h>

#include "config.h"
#include "logger.h"
#include "dynamicLinker.h"
#include "utfconv.h"
#include "dataHelper.h"
#include "redisManager.h"

#endif
