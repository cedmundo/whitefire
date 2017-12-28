#ifndef WFE_POOL_H
#define WFE_POOL_H
#include <wfe/types.h>

#define WFE_POOL_TINY   ((wfeSize) 512)         // 512 B
#define WFE_POOL_SMALL  ((wfeSize) 1024)        // 1 KB
#define WFE_POOL_MEDIUM ((wfeSize) 1048576)     // 1 MB
#define WFE_POOL_LARGE  ((wfeSize) 10485760)    // 10 MB
#define WFE_POOL_HUGE   ((wfeSize) 104857600)   // 100 MB

#define WFE_POOL_OMEM_BLOCK WFE_MAKE_MEMORY_ERROR(11)
#define WFE_POOL_OMEM_TIER WFE_MAKE_MEMORY_ERROR(12)
#define WFE_POOL_OMEM_POOL WFE_MAKE_MEMORY_ERROR(13)

/** \brief References for a chunk memory allocation.
 */
typedef struct wfePoolBlock {
    struct wfePoolBlock *next; // Next block
    wfeData *start; // Start of block
    wfeData *head;  // Current available memory
    wfeData *end;   // End of block (start+size)
} wfePoolBlock;

/** \brief A size-based tier of chunks.
 */
typedef struct wfePoolTier {
    wfePoolBlock *first, *current;
    wfeSize size;
} wfePoolTier;

/** \brief Memory pool.
 *
 * General propouse memory pool, implemented using
 * object tiers and without threading support.
 */
typedef struct wfePool {
    wfePoolTier* tiny;
    wfePoolTier* small;
    wfePoolTier* medium;
    wfePoolTier* large;
    wfePoolTier* huge;
} wfePool;

#endif
