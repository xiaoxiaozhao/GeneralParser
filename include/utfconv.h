#ifndef UTFCONV_H
#define UTFCONV_H

#include <iconv.h>

int code_convert(char *from_charset, char *to_charset, char *inbuf,  size_t inlen, unsigned char *outbuf,  size_t outlen);

int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

#endif
