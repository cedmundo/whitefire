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
