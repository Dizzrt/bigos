#ifndef __BIG_KBITSET_H__
#define __BIG_KBITSET_H__

#include "stdint.h"

class kbitset {
private:
  inline bool isFree(uint64_t);

protected:
  uint8_t* _bp;    // bit pointer
  uint64_t _len;  // max count

public:
  uint64_t _fc;  // free count

  kbitset(uint8_t* bp, uint64_t len) : _bp(bp), _len(len), _fc(_len) {}
  ~kbitset() = default;

  void set(uint64_t);            // offset
  void set(uint64_t, uint64_t);  // offset length

  void reset(uint64_t);
  void reset(uint64_t, uint64_t);

  void fill() { set(0, _len); }
  void clear() { reset(0, _len); };

  uint64_t scan(uint64_t);
  inline void init(uint8_t*, uint64_t);
};

#endif //__BIG_KBITSET_H__