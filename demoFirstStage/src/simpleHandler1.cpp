#include "../../include/common.h"
#include "../include/handler.h"
#include "../include/simpleHandler1.h"

void copyString(char** dest, char* src)
{
  int len = strlen(src);
  *dest = (char*)malloc(len + 1);
  (*dest)[len] = '\0';
  memcpy(*dest, src, len);
}

SimpleHandler1::~SimpleHandler1()
{
}

char* SimpleHandler1::spaceLink(char* str1, char* str2)
{
  int len = strlen(str1) + strlen(str2);
  char* ret = (char*)malloc(len + 2);
  ret[0] = '\0';
  strcat(ret, str1);
  strcat(ret, " ");
  strcat(ret, str2);

  helper->escapeStr(ret);

  return ret;
}
//string linker

char* SimpleHandler1::getKV(char* key, char* value)
{
  helper->escapeStr(key);
  helper->escapeStr(value);

  int len = strlen(key) + strlen(value);
  char* ret = (char*)malloc(len + 2);
  ret[0] = '\0';
  strcat(ret, key);
  strcat(ret, "=");
  strcat(ret, value);

  return ret;
}
