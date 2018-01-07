#include <wfe/pool.h>
#include <wfe/types.h>
#include <stdlib.h>
#include <assert.h>

wfeError wfePoolBlockInit(wfePoolBlock *block, wfeSize size) {
    wfeData *mem = NULL;
    assert(block != NULL /* block should not be null */);
    assert(size > 0 /* size should be at least 1 */);

    // Allocate and empty memory for block.
    // TODESIGN: Is a good idea to include a memory profile?
    mem = calloc(1, size);
    if (mem == NULL)
        return WFE_POOL_OMEM_CHUNK;

    // Set initial state for block.
    block->start = mem;
    block->head = mem;
    block->next = NULL;
    block->end = mem+size;
    return WFE_SUCCESS;
}

void wfePoolBlockFinalize(wfePoolBlock *block) {
    assert(block != NULL /* block should not be null */);

    // Free memory only if it haven't been released yet.
    if (block->start != NULL){
        free(block->start);
    }

    block->next = NULL;
    block->start = NULL;
    block->head = NULL;
    block->end = NULL;
}

wfeSize wfePoolBlockAvailable(wfePoolBlock *block) {
    assert(block != NULL /* block should not be null */);
    return block->end - block->head;
}

wfeSize wfePoolBlockTotalSize(wfePoolBlock *block) {
    assert(block != NULL /* block should not be null */);
    return block->end - block->start;
}

wfeError wfePoolTierInit(wfePoolTier *tier, wfeSize size) {
    wfeError status = WFE_SUCCESS;
    assert(tier != NULL /* block should not be null */);
    assert(size > 0 /* size should be at least 1 */);

    tier->first = NULL;
    tier->current = NULL;
    tier->size = size;

    // Expand chain to first chunk.
    tier->first = malloc(sizeof(wfePoolBlock));
    if (tier->first == NULL) {
        status = WFE_POOL_OMEM_BLOCK; goto finalize;
    }

    // Initialize first chunk and point current to first.
    status = wfePoolBlockInit(tier->first, size);
    tier->current = tier->first;

finalize:
    if (WFE_HAS_FAILED(status) && tier->first != NULL) {
        free(tier->first);
    }

    return status;
}

void wfePoolTierFinalize(wfePoolTier *tier) {
    wfePoolBlock *tmp, *cur = tier->first;
    assert(tier != NULL /* block should not be null */);

    // Finalize each block on chain.
    while (cur != NULL) {
        tmp = cur->next;

        wfePoolBlockFinalize(cur);
        free(cur); // Release the header.
        cur = tmp;
    }

    tier->first = NULL;
    tier->current = NULL;
}

wfeSize wfePoolTierAvailable(wfePoolTier *tier) {
    assert(tier != NULL /* tier should not be null */);
    if (tier->current == NULL)
        return (wfeSize) 0L;

    return wfePoolBlockAvailable(tier->current);
}

wfeSize wfePoolTierTotalSize(wfePoolTier *tier) {
    wfePoolBlock *cur = NULL;
    wfeSize len = 0;
    assert(tier != NULL /* tier should not be null */);

    if (tier->current == NULL)
        return (wfeSize) 0L;

    cur = tier->first;
    while(cur != NULL) {
        len += wfePoolBlockTotalSize(cur);
        cur = cur->next;
    }

    return len;
}

wfeData *wfePoolTierGet(wfePoolTier *tier, wfeSize size, wfeSize align) {
    wfePoolBlock *tmp = NULL;
    wfeData *ptr = NULL;
    wfeError res = WFE_SUCCESS;
    assert(tier != NULL /* tier must not be null */);
    assert(size > 0 /* Size must be at least 1 */);

    // Align memory to prevent hardware exceptions or slowness
    size = wfePoolMemoryAlign(size, align);

    // Allocate a bigger chunk if required size is major than current chunksize
    if (size > tier->size) {
        tmp = malloc(sizeof(wfePoolBlock));
        res = wfePoolBlockInit(tmp, size);
        if (WFE_HAS_FAILED(res)) {
            if (tmp != NULL) {
                free(tmp);
            }

            return NULL;
        }

        // If there is recycled memory use int, only insert bigger chunk.
        if (tier->current->next != NULL) {
            tmp->next = tier->current->next;
        }

        // Insert block on the chain.
        tier->current->next = tmp;
        tier->current = tmp;
    }

    tmp = tier->current;
    ptr = tmp->head; // Usable memory
    tmp->head += size; // Move head to next block.

    // Check if current block is exhausted.
    if (tmp->head >= tmp->end) {

        // Is the current next block already allocated?
        if (tmp->next != NULL){
            // Re-use block.
            tmp->next->head = tmp->next->start;
            tier->current = tmp->next;
        } else {
            // Otherwise extend tier with new block
            tmp->next = malloc(sizeof(wfePoolBlock));
            res = wfePoolBlockInit(tmp->next, size);
            if (WFE_HAS_FAILED(res)) {
                if (tmp->next != NULL) {
                    free(tmp->next);
                }

                return NULL;
            }

            tier->current = tmp->next;
        }

        // Set ptr to the first location in the new block
        ptr = tier->current->head;
        tier->current->head += size;
    }

    return ptr;
}

void wfePoolTierRecycle(wfePoolTier *tier) {
    assert(tier != NULL /* tier must not be null */);
    tier->current = tier->first;
    tier->current->head = tier->current->start;
}

wfeError wfePoolInit(wfePool *pool) {
    assert(pool != NULL /* pool must not be null */);
    pool->tiny = NULL;
    pool->small = NULL;
    pool->medium = NULL;
    pool->large = NULL;
    pool->huge = NULL;
    pool->custom = NULL;
    pool->fixed = NULL;
    pool->threshold = 0.25;

    return WFE_SUCCESS; // Only to keep convention
}

void wfePoolFinalize(wfePool *pool) {
    assert(pool != NULL /* pool must not be null */);

    if (pool->tiny != NULL) {
        wfePoolTierFinalize(pool->tiny);
        free(pool->tiny); // Release handler
        pool->tiny = NULL;
    }

    if (pool->small != NULL) {
        wfePoolTierFinalize(pool->small);
        free(pool->small); // Release handler
        pool->small = NULL;
    }

    if (pool->medium != NULL) {
        wfePoolTierFinalize(pool->medium);
        free(pool->medium); // Release handler
        pool->medium = NULL;
    }

    if (pool->large != NULL) {
        wfePoolTierFinalize(pool->large);
        free(pool->large); // Release handler
        pool->large = NULL;
    }

    if (pool->huge != NULL) {
        wfePoolTierFinalize(pool->huge);
        free(pool->huge); // Release handler
        pool->huge = NULL;
    }

    if (pool->custom != NULL) {
        wfePoolTierFinalize(pool->custom);
        free(pool->custom); // Release handler
        pool->custom = NULL;
    }

    if (pool->fixed != NULL) {
        wfePoolTierFinalize(pool->fixed);
        free(pool->fixed); // Release handler
        pool->fixed = NULL;
    }
}

wfeError wfePoolFixedTier(wfePool *pool, wfeSize size) {
    assert(pool != NULL /* pool must not be null */);
    pool->fixed = malloc(sizeof(wfePoolTier));
    if (pool->fixed == NULL) {
        return WFE_POOL_OMEM_TIER;
    }

    return wfePoolTierInit(pool->fixed, size);
}

wfeSize wfePoolTotalSize(wfePool *pool) {
    wfeSize total = 0;
    assert(pool != NULL /* pool must not be null */);

    if (pool->tiny != NULL)
        total += wfePoolTierTotalSize(pool->tiny);

    if (pool->small != NULL)
        total += wfePoolTierTotalSize(pool->small);

    if (pool->medium != NULL)
        total += wfePoolTierTotalSize(pool->medium);

    if (pool->large != NULL)
        total += wfePoolTierTotalSize(pool->large);

    if (pool->huge != NULL)
        total += wfePoolTierTotalSize(pool->huge);

    if (pool->custom != NULL)
        total += wfePoolTierTotalSize(pool->custom);

    if (pool->fixed != NULL)
        total += wfePoolTierTotalSize(pool->fixed);

    return total;
}

#define checkSize(p, s, tier) ( ((wfeSize) (tier)*(p->threshold)) >= (s) )
wfeData *wfePoolGet(wfePool *pool, wfeSize size, wfeSize align) {
    assert(pool != NULL /* pool must not be null */);

    if (pool->fixed != NULL)
        return wfePoolTierGet(pool->fixed, size, align);

    if (checkSize(pool, size, WFE_POOL_TINY)) {
        if (pool->tiny == NULL) {
            pool->tiny = malloc(sizeof(wfePoolTier));
            if (pool->tiny == NULL) {
                pool->lastError = WFE_POOL_OMEM_TIER;
                return NULL;
            }

            wfeError inierr = wfePoolTierInit(pool->tiny, WFE_POOL_TINY);
            if (WFE_HAS_FAILED(inierr)) {
                pool->lastError = inierr;
                return NULL;
            }
        }

        return wfePoolTierGet(pool->tiny, size, align);
    }

    if (checkSize(pool, size, WFE_POOL_SMALL)) {
        if (pool->small == NULL) {
            pool->small = malloc(sizeof(wfePoolTier));
            if (pool->small == NULL) {
                pool->lastError = WFE_POOL_OMEM_TIER;
                return NULL;
            }

            wfeError inierr = wfePoolTierInit(pool->small, WFE_POOL_SMALL);
            if (WFE_HAS_FAILED(inierr)) {
                pool->lastError = inierr;
                return NULL;
            }
        }

        return wfePoolTierGet(pool->small, size, align);
    }

    if (checkSize(pool, size, WFE_POOL_MEDIUM)) {
        if (pool->medium == NULL) {
            pool->medium = malloc(sizeof(wfePoolTier));
            if (pool->medium == NULL) {
                pool->lastError = WFE_POOL_OMEM_TIER;
                return NULL;
            }

            wfeError inierr = wfePoolTierInit(pool->medium, WFE_POOL_MEDIUM);
            if (WFE_HAS_FAILED(inierr)) {
                pool->lastError = inierr;
                return NULL;
            }
        }

        return wfePoolTierGet(pool->medium, size, align);
    }

    if (checkSize(pool, size, WFE_POOL_LARGE)) {
        if (pool->large == NULL) {
            pool->large = malloc(sizeof(wfePoolTier));
            if (pool->large == NULL) {
                pool->lastError = WFE_POOL_OMEM_TIER;
                return NULL;
            }

            wfeError inierr = wfePoolTierInit(pool->large, WFE_POOL_LARGE);
            if (WFE_HAS_FAILED(inierr)) {
                pool->lastError = inierr;
                return NULL;
            }
        }

        return wfePoolTierGet(pool->large, size, align);
    }

    if (checkSize(pool, size, WFE_POOL_HUGE)) {
        if (pool->huge == NULL) {
            pool->huge = malloc(sizeof(wfePoolTier));
            if (pool->huge == NULL) {
                pool->lastError = WFE_POOL_OMEM_TIER;
                return NULL;
            }

            wfeError inierr = wfePoolTierInit(pool->huge, WFE_POOL_HUGE);
            if (WFE_HAS_FAILED(inierr)) {
                pool->lastError = inierr;
                return NULL;
            }
        }

        return wfePoolTierGet(pool->huge, size, align);
    }

    if (pool->custom == NULL) {
        pool->custom = malloc(sizeof(wfePoolTier));
        if (pool->custom == NULL) {
            pool->lastError = WFE_POOL_OMEM_TIER;
            return NULL;
        }

        // Firstly get a tiny chunk, next chunks will have custom size (very large).
        wfeError inierr = wfePoolTierInit(pool->custom, WFE_POOL_TINY);
        if (WFE_HAS_FAILED(inierr)) {
            pool->lastError = inierr;
            return NULL;
        }
    }

    return wfePoolTierGet(pool->custom, size, align);
}

#undef checkSize

void wfePoolRecycle(wfePool *pool) {
    assert(pool != NULL /* pool must not be null */);

    if (pool->tiny != NULL)
        wfePoolTierRecycle(pool->tiny);

    if (pool->small != NULL)
        wfePoolTierRecycle(pool->small);

    if (pool->medium != NULL)
        wfePoolTierRecycle(pool->medium);

    if (pool->large != NULL)
        wfePoolTierRecycle(pool->large);

    if (pool->huge != NULL)
        wfePoolTierRecycle(pool->huge);

    if (pool->custom != NULL)
        wfePoolTierRecycle(pool->custom);

    if (pool->fixed != NULL)
        wfePoolTierRecycle(pool->fixed);
}
