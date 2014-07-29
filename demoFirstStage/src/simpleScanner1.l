%option noyywrap yylineno
%{
  #include "../../include/common.h"
  #include <iostream>
  #include "../../include/yyInput.h"
  #include "simpleParser1.tab.h"
  #include "../include/handler.h"
  #include "../include/simpleHandler1.h"

  extern Logger* logger;

#define YY_INPUT(buf, result, max_size) {\
  result = readInput(buf, max_size); \
  if (result <= 0) \
    result = YY_NULL; \
}
%}

%%

[0-9]+ { copyString(&(yylval.ustr), yytext); return INT; }
[0-9]{4}"/"[0-9]{2}"/"[0-9]{2} { copyString(&(yylval.ustr), yytext); return DATE; }
[0-9]{2}":"[0-9]{2}":"[0-9]{2} { copyString(&(yylval.ustr), yytext); return TIME; }
[A-Z][0-9]+ { copyString(&(yylval.ustr), yytext); return TERM; }
[0-9]+\.[0-9]+ { copyString(&(yylval.ustr), yytext); return FLOAT; }
[\t ]+ { return SPACE; }
\r\n { return EOL; }
([^0-9a-z\t\r\n .:/]+[0-9]*)+  { copyString(&(yylval.ustr), yytext); return WORD; }
([^0-9a-z\t\r\n ./]+"："[^0-9a-z\t\r\n ./]+" "?)+  {copyString(&(yylval.ustr), yytext); return WORD2; }
[^0-9a-z\t\r\n .:/]+[\t ]{3}[^0-9a-z\t\r\n :./]+  {copyString(&(yylval.ustr), yytext); return WORD1; }

%%
// empty
//([^+])+  { copyString(&(yylval.ustr), yytext); return UNKNOW; }
