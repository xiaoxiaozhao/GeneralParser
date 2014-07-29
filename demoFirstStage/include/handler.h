#ifndef HANDLER_H
#define HANDLER_H

class Handler
{
  public:
  Handler(Logger* logger, RedisManager* redisMgr)
  {
    this->logger = logger;
    this->redisMgr = redisMgr;
  }
  virtual ~Handler()
  {
    // do nothing
  }

  Logger* logger;
  RedisManager* redisMgr;
};

#endif
