#ifndef __BIG_MEMPOOL_H
#define __BIG_MEMPOOL_H

#include "KTL\bitset.h"
//#include "KTL\list.h"

class AddressPool : bitset {
  private:
    uint64_t s_address;
    uint64_t freeCnt;

  public:
    AddressPool(/* args */);
    ~AddressPool();
};

class VAPool {
  private:
    uint64_t m_VA;

  public:
    VAPool(/* args */);
    ~VAPool();
};

#endif