#ifndef WFE_POOL_H
#define WFE_POOL_H
#include <wfe/types.h>

#define WFE_POOL_TINY   ((wfeSize) 128)         // 128 B
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
    wfePoolTier* custom;
    wfePoolTier* fixed;
    wfeNum threshold;
    wfeError lastError;
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

/**
 * Initializes a memory pool.
 *
 * Only sets to zero value each tier so they can be marked as uninitialized.
 *
 * Params:
 *  - pool to initialize.
 * Return:
 *  - WFE_SUCCESS, always for version 1.0.
 */
wfeError wfePoolInit(wfePool *pool);

/**
 * Releases all resources of a pool.
 *
 * Finalizes all used tiers calling wfePoolTierFinalize for each, which should
 * also release all chunk chains.
 *
 * Warning: do not write/read on any reference that points to data stored
 * on finalized pool. It is not guaranteed that memory is still available and
 * might throw a segment fault exeption.
 *
 * Params:
 *  - pool to finalize.
 */
void wfePoolFinalize(wfePool *pool);

/**
 * Sets the fixed tier of the pool, redirecting all memory requests to that chain.
 * Useful for objects that have the same size.
 *
 * Note: fixing size does not restrict bigger o smaller size requests, bigger requests will
 * create a custom chunk within the chain and smaller requests will sectionate current chunk.
 * However it is not recommended because many objects might severly fragment the memory blocks.
 *
 * Params:
 *  - pool to set fixed size.
 *  - size of each object.
 * Return:
 *  - WFE_SUCCESS.
 *  - WFE_POOL_OMEM_CHUNK if no memory is available to allocate first block's memory chunk (from tier).
 *  - WFE_POOL_OMEM_BLOCK if no memory is available to allocate first block's header (from tier).
 *  - WFE_POOL_OMEM_TIER if no memory is available to allocate tier's handler.
 */
wfeError wfePoolFixedTier(wfePool *pool, wfeSize size);

/**
 * Calculates the allocated memory of all pool, including all tiers with all tiers.
 *
 * Params:
 *  - pool to calculate size.
 * Returns:
 *  - Total size of allocations on bytes.
 *  - 0 if no memory have been allocated.
 */
wfeSize wfePoolTotalSize(wfePool *pool);

/**
 * Compares size parameter with each internal tier, if it's <threshold> times smaller then request
 * is assigned to that tier, pasively creating and initializing it, if success reference is returned.
 *
 * Note: if pool has a fixed size tier, then all requests are going to be redirected to that tier,
 * no matter the size.
 *
 * Warning: this methods is silent when failing, a memory failure might be present at any time on
 * this function. Use pool#lastError to get the problem.
 *
 * Params:
 *  - pool to take memory of.
 *  - size of required block.
 *  - align of type, use wfeAlignOf to get the aligment.
 * Returns:
 *  - NULL if could not allocate or initialize tier handler or memory chunk.
 *  - An aligned pointer to usable memory space (read and write, not thread safe).
 */
wfeData *wfePoolGet(wfePool *pool, wfeSize size, wfeSize align);

/**
 * Recycles all tiers of a pool. This operation does not free any memory so developer must call
 * wfePoolFinalize anyways.
 *
 * Useful (and fast) way to re-use a pool without allocating or freeing memory.
 * Params:
 *  - pool to recycle.
 */
void wfePoolRecycle(wfePool *pool);

#endif
