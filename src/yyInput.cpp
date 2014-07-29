#include "../include/common.h"
#include "../include/yyInput.h"
#include  "time.h"
char* pool = NULL;
int poolPos = 0;
int poolLen = 0;
int nCount = 0;
int getPool(void)
{
  if (pool)
  {
    free(pool);
    pool = NULL; 
  }

  // if there's no data, we'll be blocked here
  pool = redisMgr->getData();
  poolLen = strlen(pool);
  poolPos = 0;

  fflush(stdout);

  return poolLen;
}

int readInput(char* buffer, int maxBytesToRead)
{
  if (poolPos >= poolLen)
  {
    if(nCount % 2 == 0)
    {
        time_t t;
        t=time(NULL);
        printf("start%d\n", nCount);
        printf(ctime(&t));
    }
    else
    {
        printf("end%d\n", nCount);
    }
    ++nCount;
    getPool();
  }

    

  buffer[0] = pool[poolPos];
  poolPos++;

  if (buffer[0] == 0)
  {
    printf("yyInput: 0!!!!!!!!!!!!!!!\n");
    return 0;
  }
  else
    return 1;
}
