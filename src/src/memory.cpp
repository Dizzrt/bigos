/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 17:05:29
 * @LastEditTime: 2021-11-27 17:18:29
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\memory.cpp
 * @Description:
 */

#include "memory.h"
#include "io.h"
#include "string.h"
static void pbase() {
    char p[] = "base:";
    for (int i = 0; i < strlen(p); i++)
        __put_char__(p[i]);
}

static void plen() {
    char p[] = "len:";
    for (int i = 0; i < strlen(p); i++)
        __put_char__(p[i]);
}

void test() {
    uint32_t amsCount = *((uint32_t *)0x504);
    AMS *ams = (AMS *)0x508;

    char p[] = "available memory segment count:";
    for (int i = 0; i < strlen(p); i++)
        __put_char__(p[i]);
    __put_int__(amsCount, INT_MODE::DEC);
    __put_char__('\n');

    for (int i = 0; i < amsCount; i++) {
        pbase();
        __put_int__(ams[i].base, INT_MODE::HEX);
        __put_char__('\n');

        plen();
        __put_int__(ams[i].len, INT_MODE::HEX);
        __put_char__('\n');
        __put_char__('\n');
    }

    char total[] = "total available memory:";
    for (int i = 0; i < strlen(total); i++)
        __put_char__(total[i]);
    __put_int__((ams[0].len + ams[1].len) / (512 * 512), INT_MODE::DEC);
    __put_char__('M');
    __put_char__('B');
    __put_char__('\n');

    return;
}