#include "../include/common.h"

int code_convert(char *from_charset, char *to_charset,	char *inbuf,	size_t inlen,	unsigned char *outbuf,	size_t outlen)
{
        iconv_t cd;

        char **pin = &inbuf;
        unsigned char **pout = &outbuf;
        size_t len_tmp = outlen;
        
        cd = iconv_open(to_charset,from_charset);
        if (cd==0) return -1;
        memset(outbuf,0,outlen);
        if ( iconv(cd, pin, &inlen, (char**)pout, &len_tmp) == -1 ) 
                return -1;
        iconv_close(cd);

        outlen = outlen - len_tmp;
        return outlen;
}

int u2g(char *inbuf,size_t inlen, char *outbuf,size_t outlen)
{
        return code_convert("UTF-8","GBK", inbuf, inlen, (unsigned char*)outbuf, outlen);
}

int g2u(char *inbuf,size_t inlen, char *outbuf,size_t outlen)
{
        return code_convert("GBK", "UTF-8",inbuf,inlen,(unsigned char*)outbuf,outlen);
}

