#ifndef WFE_POOL_H
#define WFE_POOL_H
#include <wfe/types.h>

#define WFE_POOL_TINY   ((wfeSize) 512)         // 512 B
#define WFE_POOL_SMALL  ((wfeSize) 1024)        // 1 KB
#define WFE_POOL_MEDIUM ((wfeSize) 1048576)     // 1 MB
#define WFE_POOL_LARGE  ((wfeSize) 10485760)    // 10 MB
#define WFE_POOL_HUGE   ((wfeSize) 104857600)   // 100 MB

#define WFE_POOL_OMEM_CHUNK WFE_MAKE_MEMORY_ERROR(11)
#define WFE_POOL_OMEM_BLOCK WFE_MAKE_MEMORY_ERROR(12)
#define WFE_POOL_OMEM_TIER WFE_MAKE_MEMORY_ERROR(13)
#define wfePoolMemoryAlign(ptr,offset) ((ptr+(offset-1)) & 0xfffffffc)

/**
 * References for a chunk memory allocation.
 */
typedef struct wfePoolBlock {
    struct wfePoolBlock *next; // Next block
    wfeData *start; // Start of block
    wfeData *head;  // Current available memory
    wfeData *end;   // End of block (start+size)
} wfePoolBlock;

/**
 * A size-based tier of chunks.
 */
typedef struct wfePoolTier {
    wfePoolBlock *first, *current;
    wfeSize size;
} wfePoolTier;

/**
 * General propouse memory pool, objects are arranged
 * using size tiers.
 *
 * Warning: not thread-safe.
 */
typedef struct wfePool {
    wfePoolTier* tiny;
    wfePoolTier* small;
    wfePoolTier* medium;
    wfePoolTier* large;
    wfePoolTier* huge;
} wfePool;

/**
 * Initializes a memory block.
 *
 * Requests memory using malloc and then prepares internal state to properly
 * accept memory writes and reads using pools API.
 *
 * Note: all requested memory is automatically initialized to zero.
 * Params:
 *  - block of memory that is going to be initialized.
 *  - size of memory chunk to be requested.
 *
 * Return:
 *  - WFE_SUCCESS.
 *  - WFE_POOL_OMEM_CHUNK if no memory is available to allocate the requested chunk.
 */
wfeError wfePoolBlockInit(wfePoolBlock *block, wfeSize size);

/**
 * Releases resources of a memory block.
 *
 * Free allocated memory from initialization and resets internal values to zero.
 * Warning: Will only release resources of the requested memory block,
 *  not the entire chain so ensure to *keep a copy of next block* separately to avoid
 *  memory leaks.
 * Params:
 *  - block to finalize.
 */
void wfePoolBlockFinalize(wfePoolBlock *block);

/**
 * Returns available memory of a block.
 *
 * Calculates and return available memory on bytes.
 * Params:
 *  - block of memory to calculate.
 * Returns:
 *  - Size on bytes, zero if none is available.
 */
wfeSize wfePoolBlockAvailable(wfePoolBlock *block);

/**
 * Returns total requested memory of a block.
 *
 * Calculates and return total requested memory on bytes.
 * Params:
 *  - block of memory to calculate.
 * Returns:
 *  - Size on bytes, zero if memory have been not allocated.
 */
wfeSize wfePoolBlockTotalSize(wfePoolBlock *block);

/**
 * Initializes a block tier.
 *
 * Sets default state for tier members, requests memory for first block.
 * Params:
 *  - tier of blocks to initialize.
 *  - size of each block on tier.
 * Return:
 *  - WFE_SUCCESS.
 *  - WFE_POOL_OMEM_CHUNK if no memory is available to allocate first block's memory chunk.
 *  - WFE_POOL_OMEM_BLOCK is no memory is available to allocate first block's header.
 */
wfeError wfePoolTierInit(wfePoolTier *tier, wfeSize size);

/**
 * Releases resources of a memory block tier.
 *
 * Free allocated memory of entire block chain, including headers.
 * Params:
 *  - tier to release.
 */
void wfePoolTierFinalize(wfePoolTier *tier);

/**
 * Calculates the available memory on current block.
 *
 * Params:
 *  - tier of blocks to calculate.
 * Return:
 *  - 0 if no memory is available.
 *  - >0 bytes of available bytes.
 */
wfeSize wfePoolTierAvailable(wfePoolTier *tier);

/**
 * Calculates the total size of the entire block chain.
 *
 * Params:
 *  - tier of blocks to know the size.
 * Returns:
 *  - 0 if no memory have been allocated.
 *  - >0 bytes of allocated memory.
 */
wfeSize wfePoolTierTotalSize(wfePoolTier *tier);

/**
 * Requests and returns usable memory space allocated within the current block with
 * specified size and align. If size is bigger than maximum supported chunk size then
 * allocator will automatically request enough memory and will create a custom-size chunk.
 *
 * Params:
 *  - tier from memory is going to be taken.
 *  - size of wanted block.
 *  - align of object.
 * Returns:
 *  - NULL if could not allocate new block and tier's memory is exhausted.
 *  - An aligned pointer to usable memory space (read and write, not thread safe).
 */
wfeData *wfePoolTierGet(wfePoolTier *tier, wfeSize size, wfeSize align);

/**
 * Cleans up the registers of the tier without loosing or freeing requested memory.
 * Warning: This method does not call free at any time,
 * you also must finalize the tier after program executes.
 *
 * Params:
 *  - tier of memory to recycle.
 */
void wfePoolTierRecycle(wfePoolTier *tier);

// Pool
// wfeError wfePoolInit(wfePool *pool);
//
// void wfePoolFinalize(wfePool *pool);
//
// wfeSize wfePoolAvailable(wfePool *pool);
//
// wfeSize wfePoolTotalSize(wfePool *pool);
//
// wfeData *wfePoolGet(wfePool *pool, wfeSize size, wfeSize align);
//
// void wfePoolRecycle(wfePool *pool);
#endif
