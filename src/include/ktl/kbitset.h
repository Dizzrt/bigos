#ifndef __BIG_KBITSET_H__
#define __BIG_KBITSET_H__

#include "stdint.h"

class kbitset {
private:
  inline bool isFree(uint32_t);

protected:
  uint8_t* _bp;    // bit pointer
  uint32_t _len;  // max count

public:
  uint32_t _fc;  // free count

  kbitset(uint8_t* bp, uint32_t len) : _bp(bp), _len(len), _fc(_len) {}
  ~kbitset() = default;

  void set(uint32_t pos);            // offset
  void set(uint32_t pos, uint32_t len);  // offset length

  void reset(uint32_t pos);
  void reset(uint32_t pos, uint32_t len);

  void fill() { set(0, _len); }
  void clear() { reset(0, _len); };

  uint32_t scan(uint32_t len);

  inline void __init__(uint8_t* bp, uint32_t len);
};

#endif //__BIG_KBITSET_H__