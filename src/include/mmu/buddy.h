#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "memdef.h"

class _singleton_Buddy
{
private:
    _singleton_Buddy(/* args */) {};
    ~_singleton_Buddy() {};

    static _singleton_Buddy instance;

public:
    static _singleton_Buddy& Instance() { return instance; }

    void* alloc_pages();
    void free_pages();

};
_singleton_Buddy _singleton_Buddy::instance;

#define Buddy _singleton_Buddy::Instance()

#endif //__BIG_BUDDY_H__

