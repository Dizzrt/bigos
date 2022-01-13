#include "MMU\kmem.h"

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

map<const void*, Alloc_Header*> alloc_mp