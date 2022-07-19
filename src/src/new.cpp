#include "new.h"
#include "stddef.h"
#include "stdint.h"

extern void* kmalloc(uint32_t size);
extern void kfree(const void* p);

//---- operator new ----
void* operator new(size_t size) { return kmalloc(size); }
void* operator new[](size_t size) { return kmalloc(size); }
void operator delete(void* p) { kfree(p); }
void operator delete[](void* p) { kfree(p); }
void operator delete(void* p, unsigned long _size) { kfree(p); }

