#ifndef __BIG_NEW_H__
#define __BIG_NEW_H__

#include "stddef.h"

// void* operator new(size_t size) { return malloc(size); }
// void* operator new[](size_t size) { return malloc(size); }
// void operator delete(void* p) { free(p); }
// void operator delete[](void* p) { free(p); }

//---- placement new ----
inline void* operator new(size_t, void* p) throw() { return p; }
inline void* operator new[](size_t, void* p) throw() { return p; }
inline void operator delete(void*, void*) throw(){};
inline void operator delete[](void*, void*) throw(){};

#endif