#include "MMU\kmem.h"

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

static uint8_t static_bp_common_slab_0[512];
static uint8_t static_bp_common_slab_1[512];
static uint8_t static_bp_slab_slab_0[4096 / sizeof(Slab) / 8 + ((4096 / sizeof(Slab)) % 8 == 0 ? 0 : 1)];
static uint8_t static_bp_slab_slab_1[4096 / sizeof(Slab) / 8 + ((4096 / sizeof(Slab)) % 8 == 0 ? 0 : 1)];
static uint8_t static_bp_buddy_slab[4096 / sizeof(__buddy_node) / 8 + ((4096 / sizeof(__buddy_node)) % 8 == 0 ? 0 : 1)];

static uint8_t static_page_common_slab_0[4096];
static uint8_t static_page_common_slab_1[4096];
static uint8_t static_page_slab_slab_0[4096];
static uint8_t static_page_slab_slab_1[4096];
static uint8_t static_page_buddy_slab[4096];

Cache common_cache;
Slab common_cache_slab_0;
Slab common_cache_slab_1;
linked_container<Slab*> common_cache_slab_node_0;
linked_container<Slab*> common_cache_slab_node_1;

Cache buddy_cache;
Slab buddy_cache_slab_0;
Slab buddy_cache_slab_1;
linked_container<Slab*> buddy_cache_slab_node_0;
linked_container<Slab*> buddy_cache_slab_node_1;

Cache cache_cache;
void kmeminit() {
    uint16_t objCnt_temp;

#pragma region common cache init
    common_cache_slab_node_0.val = &common_cache_slab_0;
    common_cache_slab_node_1.val = &common_cache_slab_1;

    common_cache.partial.__list_add(&common_cache_slab_node_0, common_cache.partial.end());
    common_cache.empty.__list_add(&common_cache_slab_node_1, common_cache.empty.end());

    new (&common_cache_slab_0) Slab(128, 0x1000, (uint64_t)static_page_common_slab_0, static_bp_common_slab_0);
    new (&common_cache_slab_1) Slab(128, 0x1000, (uint64_t)static_page_common_slab_1, static_bp_common_slab_1);
// common_cache init complete,kmalloc can be used
#pragma endregion

#pragma region buddy cache init
    buddy_cache_slab_node_0.val = &buddy_cache_slab_0;
    // buddy_cache_slab_node_1.val = &buddy_cache_slab_1;

    buddy_cache.partial.__list_add(&buddy_cache_slab_node_0, buddy_cache.partial.end());
    // buddy_cache.empty.__list_add(&buddy_cache_slab_node_1, buddy_cache.empty.end());

    new (&buddy_cache_slab_0)
        Slab(128, 0x1000 / sizeof(__buddy_node), (uint64_t)static_page_buddy_slab, static_bp_buddy_slab);
    // new (&buddy_cache_slab_1) Slab(128, 0x1000 / sizeof(__buddy_node), 0xffff800000004000);
#pragma endregion

#pragma region slab cache init
    slab_cache_slab_node_0.val = &slab_cache_slab_0;
    slab_cache_slab_node_1.val = &slab_cache_slab_1;

    slab_cache.partial.__list_add(&slab_cache_slab_node_0, slab_cache.partial.end());
    slab_cache.empty.__list_add(&slab_cache_slab_node_1, slab_cache.empty.end());

    new (&slab_cache_slab_0) Slab(128, 0x1000 / sizeof(Slab), (uint64_t)static_page_slab_slab_0, static_bp_slab_slab_0);
    new (&slab_cache_slab_1) Slab(128, 0x1000 / sizeof(Slab), (uint64_t)static_page_slab_slab_1, static_bp_slab_slab_1);
#pragma endregion

    // TODO init buddy sys
}