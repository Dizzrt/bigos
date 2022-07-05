#ifndef __BIG_KASSERT_H__
#define __BIG_KASSERT_H__

#include "io.h"

#define KAssert(Expression,Message) \
    __kassert__(Expression,Message,__LINE__,__PRETTY_FUNCTION__)

void __kassert__(bool expression, const char* message,
    uint32_t line, const char* fun) {
    if (expression)
        return;

    //TODO cli
    printk_svga("error: %s\n\tline %d | %s\n", message, line, fun);
    while (true) {
        asm volatile("hlt");
    }
}

#endif //__BIG_KASSERT_H__