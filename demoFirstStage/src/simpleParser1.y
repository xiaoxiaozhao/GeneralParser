%{
  #include "../../include/common.h"
  //#define YYERROR_VERBOSE 1
  #include "../../include/yyInput.h"
  #include "../include/handler.h"
  #include "../include/simpleHandler1.h"
  #include "time.h"
  RedisManager* redisMgr;
  ParserConfig* parserConfig;
  Logger* logger;
  DataHelper* dataHelper;
  
  SimpleHandler1* simpleHandler1;

  extern "C" void parserFunc(RedisManager* mgr, ParserConfig* config, Logger* log, DataHelper* help);

  void yyerror(char* s);
  extern int yylex(void);
  extern int yylineno;
  int count = 0;
%}

%union
{
  char* ustr;
}

%token <ustr> DATE TIME TERM WORD INT FLOAT WORD2 WORD1 SPACEL
%token SPACE EOL UNKNOW
%type <ustr> datetime branch status

%%

log : 
| log line { }
| log error { }
;

line : datetime SPACE TERM SPACE branch SPACE INT SPACE WORD SPACE INT SPACE FLOAT SPACE datetime SPACE status SPACE EOL {
  char* start = simpleHandler1->getKV("start", $1);
  char* terminal = simpleHandler1->getKV("terminal", $3);
  char* end = simpleHandler1->getKV("end", $15);

  char* ret = simpleHandler1->spaceLink(start, terminal);
  char* lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $5);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $7);
  free(lastRet);
  lastRet = ret; 
  ret = simpleHandler1->spaceLink(ret, $9);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $11);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $13);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, end);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $17);
  free(lastRet);
  lastRet = ret;

  free(start);
  free(terminal);
  free(end);
  free($1);
  free($3);
  free($5);
  free($7);
  free($9);
  free($11);
  free($13);
  free($15);
  free($17);

  simpleHandler1->helper->assemble(ret, NULL, true);

  free(ret);
//  count++;
//  printf("count=  %d\n\n",count);

}
|datetime SPACE TERM SPACE branch SPACE INT SPACE WORD SPACE INT SPACE datetime SPACE status SPACE EOL {
  char* start = simpleHandler1->getKV("start", $1);
  char* terminal = simpleHandler1->getKV("terminal", $3);
  char* end = simpleHandler1->getKV("end", $15);

  char* ret = simpleHandler1->spaceLink(start, terminal);
  char* lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $5);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $7);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $9);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $11);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $13);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, end);
  free(lastRet);
  lastRet = ret;

  free(start);
  free(terminal);
  free(end);
  free($1);
  free($3);
  free($5);
  free($7);
  free($9);
  free($11);
  free($13);
  free($15);

  simpleHandler1->helper->assemble(ret, NULL, true);

  free(ret);
//  count++;
//  printf("count=  %d\n\n",count);
};

|datetime SPACE TERM SPACE branch SPACE INT SPACE WORD SPACE INT SPACE FLOAT SPACE datetime SPACE UNKNOW {
  char* start = simpleHandler1->getKV("start", $1);
  char* terminal = simpleHandler1->getKV("terminal", $3);

  char* ret = simpleHandler1->spaceLink(start, terminal);
  char* lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $5);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $7);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $9);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $11);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, $13);
  free(lastRet);
  lastRet = ret;

  free(start);
  free(terminal);
  free($1);
  free($3);
  free($5);
  free($7);
  free($9);
  free($11);
  free($13);

  simpleHandler1->helper->assemble(ret, NULL, true);

  free(ret);
//  count++;
//  printf("count=  %d\n\n",count);
};


datetime : DATE SPACE TIME { $$ = simpleHandler1->spaceLink($1, $3); free($1); free($3); };
branch : WORD SPACE WORD {
  char* main = simpleHandler1->getKV("mainBrc", $1);
  char* sub = simpleHandler1->getKV("subBrc", $3);
  $$ = simpleHandler1->spaceLink(main, sub);
  free(main);
  free(sub);
  free($1);
  free($3);
};
status : WORD SPACE INT SPACE WORD SPACE INT{
  char* hostStat = simpleHandler1->getKV("hostStat", $1);
  char* termCode = simpleHandler1->getKV("termCode", $3);
  char* codeDesc = simpleHandler1->getKV("codeDesc", $5);
  char* termSerial = simpleHandler1->getKV("termSerial", $7);
  char* ret = simpleHandler1->spaceLink(hostStat, termCode);
  char* lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, codeDesc);
  free(lastRet);
  lastRet = ret;
  ret = simpleHandler1->spaceLink(ret, termSerial);
  free(lastRet);
  free(hostStat);
  free(termCode);
  free(codeDesc);
  free(termSerial);
  free($1);
  free($3);
  free($5);
  free($7);
  $$ = ret;
}
| status SPACE INT{$$=$1;}
| WORD SPACE WORD SPACE WORD {$$=$1;}
| status SPACE  WORD2 {	$$=$1;}
| WORD1 {$$=$1;}
;


%%

void parserFunc(RedisManager* mgr, ParserConfig* config, Logger* log, DataHelper* help)
{ 
  //clock_t start,finish;
//  double d;
  redisMgr = mgr;
  parserConfig = config;
  logger = log;
  dataHelper = help;

  simpleHandler1 = new SimpleHandler1(dataHelper, logger, redisMgr);

  //yydebug = 1;
  
  {
    yyparse();
    fflush(stdout);
  }

}

void yyerror(char* s)
{
  printf("simpleParser1.y::yyerror %d - %s\n", yylineno, s);
}
