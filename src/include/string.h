#ifndef __BIG_STRING_H__
#define __BIG_STRING_H__

#include "stddef.h"
#include "stdint.h"

#define OPSIZE 8  // align 8 byte
#define byte unsigned char

extern "C" {
void* memset(void*, int, size_t);
int memcmp(const void*, const void*, size_t);
void* memcpy(void*, const void*, size_t);

const char* strcpy(char*, const char*);
int strcmp(const char*, const char*);
size_t strlen(const char*);
// char * strchr(const char *_Str, int _Val);
// char * strrchr(const char *_Str, int _Ch);
// char * strcat(char *, const char *);

char* itoa(int64_t, char*, uint8_t);
}
#endif