#include "MMU\kmem.h"

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

#define GET_BP_SIZE(ObjSize) 0x200 / ObjSize + ((0x1000 / ObjSize) % 8 == 0 ? 0 : 1)
#define STATIC_SLAB(Name, ObjSize)                             \
    static uint8_t StaticSlab_Page_##Name[0x1000];             \
    static uint8_t StaticSlab_Bp_##Name[GET_BP_SIZE(ObjSize)]; \
    static linked_container<Slab> StaticSlab_Node_##Name;

STATIC_SLAB(Common_0, 1)
STATIC_SLAB(Common_1, 1)
STATIC_SLAB(Slab_0, sizeof(slab_container))
STATIC_SLAB(Slab_1, sizeof(slab_container))
STATIC_SLAB(Buddy, sizeof(buddy_container))
STATIC_SLAB(BuddyMap_0, sizeof(buddyMapNode))
STATIC_SLAB(BuddyMap_1, sizeof(buddyMapNode))

Cache slab_cache;
Cache buddy_cache;
Cache common_cache;
Cache buddyMapNode_cache;

void kmeminit() {
#pragma region common cache init
    common_cache.partial.__list_insert(&StaticSlab_Node_Common_0, common_cache.partial.end());
    common_cache.empty.__list_insert(&StaticSlab_Node_Common_1, common_cache.empty.end());

    new (&StaticSlab_Node_Common_0.val) Slab(128, 0x1000, (uint64_t)StaticSlab_Page_Common_0, StaticSlab_Bp_Common_0);
    new (&StaticSlab_Node_Common_1.val) Slab(128, 0x1000, (uint64_t)StaticSlab_Page_Common_1, StaticSlab_Bp_Common_1);
// common_cache init complete,kmalloc can be used
#pragma endregion

#pragma region buddy cache init
    buddy_cache.partial.__list_insert(&StaticSlab_Node_Buddy, buddy_cache.partial.end());

    new (&StaticSlab_Node_Buddy.val)
        Slab(128, 0x1000 / sizeof(__buddy_node), (uint64_t)StaticSlab_Page_Buddy, StaticSlab_Bp_Buddy);
#pragma endregion

#pragma region buddyMap cache init
    buddyMapNode_cache.partial.__list_insert(&StaticSlab_Node_BuddyMap_0, buddyMapNode_cache.partial.end());
    buddyMapNode_cache.empty.__list_insert(&StaticSlab_Node_BuddyMap_1, buddyMapNode_cache.empty.end());

    new (&StaticSlab_Node_BuddyMap_0)
        Slab(128, 0x1000 / sizeof(buddyMapNode), (uint64_t)StaticSlab_Page_BuddyMap_0, StaticSlab_Bp_BuddyMap_0);
    new (&StaticSlab_Node_BuddyMap_1)
        Slab(128, 0x1000 / sizeof(buddyMapNode), (uint64_t)StaticSlab_Page_BuddyMap_1, StaticSlab_Bp_BuddyMap_1);
#pragma endregion

#pragma region slab cache init
    slab_cache.partial.__list_insert(&StaticSlab_Node_BuddyMap_0, slab_cache.partial.end());
    slab_cache.empty.__list_insert(&StaticSlab_Node_BuddyMap_1, slab_cache.empty.end());

    new (&StaticSlab_Node_BuddyMap_0.val)
        Slab(128, 0x1000 / sizeof(Slab), (uint64_t)StaticSlab_Page_Slab_0, StaticSlab_Bp_Slab_0);
    new (&StaticSlab_Node_BuddyMap_1.val)
        Slab(128, 0x1000 / sizeof(Slab), (uint64_t)StaticSlab_Page_Slab_1, StaticSlab_Bp_Slab_1);
#pragma endregion

    __buddy_free(0x40000, 64);  // the initial 64 available pages
}