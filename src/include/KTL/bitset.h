#ifndef __BIG_BITSET_H__
#define __BIG_BITSET_H__

#include "stdint.h"

class bitset {
  protected:
    uint8_t* _bp;    // bit pointer
    uint64_t _size;  // max count

  public:
    uint64_t _fc;  // free count

    bitset(uint8_t* bp, uint64_t __size) : _bp(bp), _size(__size), _fc(_size){};
    bitset(uint64_t __size) { resize(__size); };
    bitset() : bitset(8){};
    //~bitset(); //MARKER destructor

    void set(uint64_t);            // offset
    void set(uint64_t, uint64_t);  // offset length
    void reset(uint64_t);
    void reset(uint64_t, uint64_t);

    void fill() { set(0, _size); }
    void clear() { reset(0, _size); };

    void resize(uint64_t);

    uint64_t scan(uint64_t size);
};

#endif