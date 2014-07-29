#ifndef YYINPUT_H
#define YYINPUT_H

// include this header in .l file
//#undef YY_INPUT

extern Logger* logger;
extern RedisManager* redisMgr;
extern DataHelper* dataHelper;

int readInput(char* buffer, int maxBytesToRead);

#endif
