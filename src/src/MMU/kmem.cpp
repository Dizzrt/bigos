#include "MMU\kmem.h"

// void* v2p(void* p) { return v2p((uint64_t)p); }
// void* v2p(uint64_t p) {
//     uint16_t _p[5];                          // split the virtual address
//     _p[0] = (p & 0x0000ff8000000000) >> 39;  // pml4
//     _p[1] = (p & 0x0000007fc0000000) >> 30;  // pdpt
//     _p[2] = (p & 0x000000003fe00000) >> 21;  // pd
//     _p[3] = (p & 0x00000000001ff000) >> 12;  // pt
//     _p[4] = (p & 0x0000000000000fff);        // offset

//     uint64_t pdbr;
//     asm volatile("movq %%cr3,%0" : "=m"(pdbr));
//     pdbr = (pdbr & 0xfffffffffffff000) >> 12;
//     pdbr = (uint64_t)p2v((void*)pdbr);  // the virtual address of the pml4 entry

//     uint64_t ret = ((uint64_t****)pdbr)[_p[0]][_p[1]][_p[2]][_p[3]] + _p[4];
//     return (void*)ret;
// }

// void* p2v(void* p) { return p2v((uint64_t)p); }
// void* p2v(uint64_t p) {}