/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-30 11:26:22
 */
#include "MMU\buddy.h"

list<__buddy_node *> blist[11];

// void pbuddy() {
//     for (int i = 0; i < 11; i++) {
//         printk_svga("block size:%d\tblock count:%d\n", (1 << i), blist[i].size());
//     }
// }