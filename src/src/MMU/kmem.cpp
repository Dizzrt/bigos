#include "MMU\kmem.h"

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

#define GET_OBJ_COUNT(OBJSIZE) 0x1000 / (sizeof(SlabHeader) + OBJSIZE)
#define GET_BP_SIZE(OBJSIZE) GET_OBJ_COUNT(OBJSIZE) / 8 + (GET_OBJ_COUNT(OBJSIZE) % 8 == 0 ? 0 : 1)
#define STATIC_SLAB(NAME, OBJSIZE)                             \
    static uint8_t StaticSlab_Page_##NAME[0x1000];             \
    static uint8_t StaticSlab_Bp_##NAME[GET_BP_SIZE(OBJSIZE)]; \
    static linked_container<Slab> StaticSlab_##NAME;

#define CACHE(NAME)     \
    Cache cache_##NAME; \
    linked_container<Cache> canode_##NAME;

STATIC_SLAB(Slab_0, sizeof(slab_container))
STATIC_SLAB(Slab_1, sizeof(slab_container))
STATIC_SLAB(Buddy, sizeof(buddy_container))
STATIC_SLAB(BuddyMap_0, sizeof(buddyMapNode))
STATIC_SLAB(BuddyMap_1, sizeof(buddyMapNode))

CACHE(16)
CACHE(32)
CACHE(64)
CACHE(128)
CACHE(256)
CACHE(512)
CACHE(1024)
CACHE(slab)
CACHE(buddy)
CACHE(buddyMapNode)

CacheChain kmem_cache;

void kmeminit() {
    kmem_cache._caList.__list_insert(&canode_16, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_32, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_64, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_128, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_256, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_512, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_1024, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_slab, kmem_cache._caList.end());
    kmem_cache._caList.__list_insert(&canode_buddy, kmem_cache._caList.end());

    cache_buddy.partial.__list_insert(&StaticSlab_Buddy, cache_buddy.partial.end());
    cache_buddyMapNode.partial.__list_insert(&StaticSlab_BuddyMap_0, cache_buddyMapNode.partial.end());
    cache_buddyMapNode.empty.__list_insert(&StaticSlab_BuddyMap_1, cache_buddyMapNode.empty.end());
    cache_slab.partial.__list_insert(&StaticSlab_Slab_0, cache_slab.partial.end());
    cache_slab.empty.__list_insert(&StaticSlab_Slab_1, cache_slab.empty.end());

    new (&StaticSlab_Buddy.val)
        Slab(128, GET_OBJ_COUNT(sizeof(buddy_container)), (uint64_t)StaticSlab_Page_Buddy, StaticSlab_Bp_Buddy);
    new (&StaticSlab_BuddyMap_0)
        Slab(128, GET_OBJ_COUNT(sizeof(buddyMapNode)), (uint64_t)StaticSlab_Page_BuddyMap_0, StaticSlab_Bp_BuddyMap_0);
    new (&StaticSlab_BuddyMap_1)
        Slab(128, GET_OBJ_COUNT(sizeof(buddyMapNode)), (uint64_t)StaticSlab_Page_BuddyMap_1, StaticSlab_Bp_BuddyMap_1);
    new (&StaticSlab_Slab_0.val)
        Slab(128, GET_OBJ_COUNT(sizeof(slab_container)), (uint64_t)StaticSlab_Page_Slab_0, StaticSlab_Bp_Slab_0);
    new (&StaticSlab_Slab_1.val)
        Slab(128, GET_OBJ_COUNT(sizeof(slab_container)), (uint64_t)StaticSlab_Page_Slab_1, StaticSlab_Bp_Slab_1);

    __buddy_free(0x40000, 64);  // the initial 64 available pages
}