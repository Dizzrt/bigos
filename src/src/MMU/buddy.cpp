#include "mmu\buddy.h"
#include "io.h"

static klist<uint64_t> buddyBlocks[11];
static _rb_tree<void*, buddyBlock> buddy_mp;

static uint16_t bls[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
static uint32_t blsx[] = { 1 * 0x1000, 2 * 0x1000, 4 * 0x1000, 8 * 0x1000, 16 * 0x1000,
                            32 * 0x1000, 64 * 0x1000, 128 * 0x1000, 256 * 0x1000, 512 * 0x1000, 1024 * 0x1000 };

void* buddy_alloc(unsigned int pCount) {
    if (pCount > 1024)
        return nullptr;

    int t = 0;
    while (pCount > bls[t])
        t++;

    int rlen = bls[t] - pCount;
    linked_container_iterator<uint64_t> iter = buddyBlocks[t].begin();
    buddyBlocks[t].__list_remove(iter);

    uint64_t np = *iter + 0x1000 * pCount;
    for (int i = 10; rlen > 0 && i >= 0; i++) {
        while (rlen >= bls[i]) {
            // TODO alloc
            // buddyBlocks[i].__list_insert()
            np += 0x1000 * bls[i];
        }
    }

    np = *iter;
    // TODO free iter.m_node
    return (void*)np;
}

void buddy_free(uint64_t base, uint64_t len) {
    for (int i = 10;i >= 0;i--) {
        while (len >= blsx[i])
        {
            linked_container<uint64_t>* temp;
            temp = (linked_container<uint64_t>*)getLC_8B();
            temp->val = base;
            buddyBlocks[i].__list_insert(temp); //TODO priority_queue

            base += blsx[i];
            len -= blsx[i];
        }
    }
}

void buddy_free(const void* p) {}

void buddyInit() {
    struct AMS  // available memory segment
    {
        uint64_t base;
        uint64_t len;
        uint32_t type;
        // Type 1: Usable (normal) RAM
        // Type 2: Reserved - unusable
        // Type 3: ACPI reclaimable memory
        // Type 4: ACPI NVS memory
        // Type 5: Area containing bad memory
        uint32_t alignment; // member that should not be used
    };

    AMS* _ams = (AMS*)0xffff800000000580;
    int _amsCount = *((int*)0xffff800000000500);
    uint64_t ksize = (uint64_t)(*(uint32_t*)0xffff800000000504);

    // 4096 bytes per page; 512 bytes per sector
    ksize = ksize / 8 + (ksize % 8 == 0 ? 0 : 1);  // pages of kernel

    // 512 pages per PT
    uint64_t tsize = ksize / 512 + (ksize % 512 == 0 ? 0 : 1); // PTs
    tsize *= 512;// total pages that used by the kernel and mapped but not used

    //start position of available memory segment
    //0x200000 is the start position of kernel
    uint64_t amsStart = tsize * 0x1000 + 0x200000;

    for (int i = 0; i < _amsCount; i++) {
        if (_ams[i].type != 1 || _ams[i].len == 0)
            continue;

        if (_ams[i].base < amsStart) {
            if (_ams[i].base + _ams[i].len < amsStart)
                continue;

            uint64_t xlen = amsStart - _ams[i].base;
            xlen = _ams[i].len - xlen;

            printk_svga("len:0x%llx\n", xlen);
            buddy_free(amsStart, xlen);
        }
        else buddy_free(_ams[i].base, _ams[i].len);
    }

    for (int i = 0;i < 11;i++) {
        klist<uint64_t> t = buddyBlocks[i];
        printk_svga("len:%d\tcount:%d\n", bls[i], t.size());
    }
}