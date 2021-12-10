/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-10 18:00:49
 */
#include "MMU\buddy.h"

list<Block *> blist[11];

void pbuddy() {
    for (int i = 0; i < 11; i++) {
        printk_svga("block size:%d\tblock count:%d\n", (1 << i), blist[i].size());
    }
}