/*
 * @Author: Dizzrt
 * @Date: 2021-11-17 15:00:41
 * @LastEditTime: 2021-12-12 15:03:32
 * @LastEditors: Please set LastEditors
 * @FilePath: \bigos\src\src\string.cpp
 * @Description:
 */

#include "string.h"

void *memset(void *dstpp, int c, size_t len) {
    long long dstp = (long long)dstpp;

    if (len >= 8) {
        unsigned long long cccc = (unsigned char)c;
        cccc |= cccc << 8;
        cccc |= cccc << 16;
        cccc |= cccc << 32;

        while (len % OPSIZE != 0) {
            ((byte *)dstp)[0] = c;
            dstp += 1;
            len -= 1;
        }

        size_t xlen = len / (OPSIZE * 8);
        while (xlen--) {
            ((size_t *)dstp)[0] = cccc;
            ((size_t *)dstp)[1] = cccc;
            ((size_t *)dstp)[2] = cccc;
            ((size_t *)dstp)[3] = cccc;
            ((size_t *)dstp)[4] = cccc;
            ((size_t *)dstp)[5] = cccc;
            ((size_t *)dstp)[6] = cccc;
            ((size_t *)dstp)[7] = cccc;
            dstp += 8 * OPSIZE;
        }
        len %= 8 * OPSIZE;
        xlen = len / OPSIZE;
        while (xlen--) {
            ((size_t *)dstp)[0] = cccc;
            dstp += OPSIZE;
        }
        len %= OPSIZE;
    }

    while (len--) {
        ((byte *)dstp)[0] = c;
        dstp++;
    }

    return dstpp;
}
int memcmp(const void *s1, const void *s2, size_t len) {
    size_t xlen = len % OPSIZE;
    len -= xlen;

    const char *bp1 = (const char *)s1;
    const char *bp2 = (const char *)s2;

    while (xlen--) {
        if (*bp1 == *bp2)
            continue;
        return *bp1 - *bp2;
    }

    xlen = len / OPSIZE;
    const long long *lp1 = (const long long *)bp1;
    const long long *lp2 = (const long long *)bp2;

    while (xlen--) {
        if (*lp1 == *lp2)
            continue;

        for (int i = 0; i < 8; i++)
            if (lp1[i] != lp2[i])
                return lp1[i] - lp2[i];
    }

    return 0;
}
void *memcpy(void *dstp, const void *srcp, size_t len) {
    size_t xlen = len % OPSIZE;
    len -= xlen;

    char *dstpp = (char *)dstp;
    const char *srcpp = (const char *)srcp;

    // 8 byte alignment
    while (xlen--)
        *dstpp++ = *srcpp++;

    xlen = len / OPSIZE;
    long long *ldstpp = (long long *)dstpp;
    long long *lsrcpp = (long long *)srcpp;

    while (xlen--)
        *ldstpp++ = *lsrcpp++;

    return dstp;
}

const char *strcpy(char *dst, const char *src) { return (const char *)memcpy(dst, src, strlen(src) + 1); }
int strcmp(const char *s1, const char *s2) {
    const unsigned char *ss1 = (const unsigned char *)s1;
    const unsigned char *ss2 = (const unsigned char *)s2;

    unsigned char c1, c2;
    do {
        c1 = (unsigned char)*ss1++;
        c2 = (unsigned char)*ss2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);

    return c1 - c2;
}
size_t strlen(const char *str) {
    const char *char_ptr;
    unsigned long long _str;
    const unsigned long long *str_ptr;

    for (char_ptr = str; ((unsigned long long)char_ptr & (sizeof(_str) - 1)) != 0; char_ptr++)
        if (*char_ptr == '\0')
            return char_ptr - str;

    str_ptr = (unsigned long long *)char_ptr;

    const unsigned long long himagic = 0x8080808080808080;
    const unsigned long long lomagic = 0x0101010101010101;

    while (true) {
        _str = *str_ptr++;
        if (((_str - lomagic) & ~_str & himagic) == 0)
            continue;

        const char *cp = (const char *)(str_ptr - 1);
        if (cp[0] == 0)
            return cp - str;
        if (cp[1] == 0)
            return cp - str + 1;
        if (cp[2] == 0)
            return cp - str + 2;
        if (cp[3] == 0)
            return cp - str + 3;
        if (cp[4] == 0)
            return cp - str + 4;
        if (cp[5] == 0)
            return cp - str + 5;
        if (cp[6] == 0)
            return cp - str + 6;
        if (cp[7] == 0)
            return cp - str + 7;
    }
}
// char * strchr(const char *_Str, int _Val);
// char * strrchr(const char *_Str, int _Ch);
// char * strcat(char *__restrict__ _Dest, const char *__restrict__ _Source);

char *itoa(int64_t val, char *str, uint8_t radix) {
    char c[] = "0123456789ABCDEF";

    uint64_t uval;
    uint32_t offset = 0;

    if (radix == 10 && val < 0) {
        uval = (uint64_t)-val;
        str[offset++] = '-';
    } else
        uval = (uint64_t)val;

    do {
        str[offset++] = c[uval % radix];
        uval /= radix;
    } while (uval);
    str[offset] = '\0';

    uint32_t flag = str[0] == '-' ? 1 : 0;
    for (uint32_t i = flag; i <= (offset - 1) / 2; i++) {
        char temp;
        temp = str[i];
        str[i] = str[offset - 1 + flag - i];
        str[offset - 1 + flag - i] = temp;
    }

    return str;
}