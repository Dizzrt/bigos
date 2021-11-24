/*
 * @Author: Dizzrt
 * @Date: 2021-11-06 17:21:31
 * @LastEditTime: 2021-11-19 13:23:41
 * @LastEditors: Dizzrt
 * @FilePath: \Big OS\src\include\string.h
 * @Description:
 */

#ifndef __BIG_STRING_H__
#define __BIG_STRING_H__

#define OPSIZE 8 // align 8 byte
#define byte unsigned char
#define size_t unsigned long long

extern "C" {
void *__cdecl memset(void *, int, size_t);
int __cdecl memcmp(const void *, const void *, size_t);
void *__cdecl memcpy(void *, const void *, size_t);

const char *__cdecl strcpy(char *, const char *);
int __cdecl strcmp(const char *, const char *);
size_t __cdecl strlen(const char *);
// char *__cdecl strchr(const char *_Str, int _Val);
// char *__cdecl strrchr(const char *_Str, int _Ch);
// char *__cdecl strcat(char *, const char *);
}
#endif