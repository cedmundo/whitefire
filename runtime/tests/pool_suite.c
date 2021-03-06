#include "minunit.h"
#include <wfe/pool.h>
#include <stdlib.h>

typedef struct _wfePoolDummy {
    char c;
    long o;
} wfePoolDummy;

static char * test_pool_block_init_finalize() {
    wfePoolBlock block;
    wfeSize size = sizeof(int);

    mu_assert("initialize memory block", wfePoolBlockInit(&block, size) == WFE_SUCCESS);
    mu_assert("unexpected block head/start", block.start == block.head);
    mu_assert("unexpected block end", (block.end - block.start) == size);

    wfePoolBlockFinalize(&block);
    mu_assert("should not reference memory after finalize", block.start == NULL);
    return 0;
}

static char * test_pool_block_size() {
    wfePoolBlock block;
    wfeSize size = sizeof(char) * 4;

    mu_assert("initialize memory block", wfePoolBlockInit(&block, size) == WFE_SUCCESS);
    mu_assert("unexpected total memory", wfePoolBlockTotalSize(&block) == size);
    mu_assert("unexpected available memory", wfePoolBlockAvailable(&block) == size);
    wfePoolBlockFinalize(&block);
    return 0;
}

static char * test_pool_tier_init_finalize() {
    wfePoolTier tier;
    wfeSize size = sizeof(char) * 2;

    mu_assert("initialize tier", wfePoolTierInit(&tier, size) == WFE_SUCCESS);
    mu_assert("invalid first block", tier.first != NULL);
    mu_assert("invalid current block", tier.first == tier.current);
    mu_assert("invalid size of first block", wfePoolBlockTotalSize(tier.first) == size);

    wfePoolTierFinalize(&tier);
    mu_assert("should not reference memory after finalize", tier.first == NULL && tier.current == NULL);
    return 0;
}

static char * test_pool_tier_size() {
    wfePoolTier tier;

    wfePoolBlock *block1 = malloc(sizeof(wfePoolBlock));
    wfePoolBlock *block2 = malloc(sizeof(wfePoolBlock));
    wfeSize size = sizeof(char) * 2;

    mu_assert("initialize memory block 1", wfePoolBlockInit(block1, size) == WFE_SUCCESS);
    mu_assert("initialize memory block 2", wfePoolBlockInit(block2, size) == WFE_SUCCESS);

    // Mock and fake block chain.
    block1->next = block2;
    tier.current = block1;
    tier.first = block1;

    // Actually test tier.
    mu_assert("unexpected total memory", wfePoolTierTotalSize(&tier) == size*2);
    mu_assert("unexpected available memory", wfePoolTierAvailable(&tier) == size);

    // Should also destroy blocks
    wfePoolTierFinalize(&tier);
    return 0;
}

static char * test_pool_tier_get() {
    wfePoolTier tier;
    wfeSize size = wfePoolMemoryAlign(sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    wfeData *data = NULL;

    mu_assert("initialize tier", wfePoolTierInit(&tier, WFE_POOL_TINY) == WFE_SUCCESS);

    // Get memory from pool tier
    data = wfePoolTierGet(&tier, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (first request)", data != NULL);
    mu_assert("data outside of current chunk space (first request)", data >= tier.current->start && data < tier.current->end);

    data = wfePoolTierGet(&tier, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (second request)", data != NULL);
    mu_assert("data outside of current chunk space (second request)", data >= tier.current->start && data < tier.current->end);

    wfePoolTierFinalize(&tier);
    return 0;
}

static char * test_pool_tier_allocate_bigger() {
    wfePoolTier tier;
    wfeSize size = sizeof(char) * 2;
    wfeSize expectedSize = wfePoolMemoryAlign(sizeof(wfePoolDummy)*2, wfeAlignOf(wfePoolDummy));

    mu_assert("initialize tier", wfePoolTierInit(&tier, size) == WFE_SUCCESS);

    // Get memory bigger than supported size.
    wfeData *data = wfePoolTierGet(&tier, sizeof(wfePoolDummy)*2, wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer", data != NULL);
    mu_assert("size of block is smaller than requested", wfePoolBlockTotalSize(tier.current) == expectedSize);

    wfePoolTierFinalize(&tier);
    return 0;
}

static char * test_pool_tier_recycle() {
    wfePoolTier tier;
    wfeData *d1 = NULL, *d2 = NULL, *d3 = NULL, *d4 = NULL;
    wfeSize size = wfePoolMemoryAlign(sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));

    mu_assert("initialize tier", wfePoolTierInit(&tier, size) == WFE_SUCCESS);
    d1 = wfePoolTierGet(&tier, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d1 request)", d1 != NULL);
    mu_assert("data outside of current chunk space (first request)", d1 >= tier.current->start && d1 < tier.current->end);

    d2 = wfePoolTierGet(&tier, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d2 request)", d2 != NULL);
    mu_assert("data outside of current chunk space (second request)", d2 >= tier.current->start && d2 < tier.current->end);

    wfePoolTierRecycle(&tier);
    d3 = wfePoolTierGet(&tier, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d3 request)", d3 != NULL);
    mu_assert("did not recycle d1", d1 == d3);

    d4 = wfePoolTierGet(&tier, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d3 request)", d4 != NULL);
    mu_assert("did not recycle d2", d2 == d4);

    wfePoolTierFinalize(&tier);
    return 0;
}

static char * test_pool_init_finalize() {
    wfePool pool;
    mu_assert("initialize pool", wfePoolInit(&pool) == WFE_SUCCESS);

    wfePoolFinalize(&pool);
    mu_assert("should not reference memory after finalize",
            pool.tiny == NULL &&
            pool.small == NULL &&
            pool.medium == NULL &&
            pool.large == NULL &&
            pool.huge == NULL);
    return 0;
}

static char * test_pool_misc_and_size() {
    wfePool pool;
    wfeSize size = wfePoolMemoryAlign(sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));

    mu_assert("initialize pool", wfePoolInit(&pool) == WFE_SUCCESS);
    mu_assert("set pool fixed size", wfePoolFixedTier(&pool, size) == WFE_SUCCESS);
    mu_assert("invalid pool size", wfePoolTotalSize(&pool) == size);

    wfePoolFinalize(&pool);
    return 0;
}

static char * test_pool_get () {
    wfePool pool;
    wfeSize s1 = wfePoolMemoryAlign(sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    wfeSize s2 = wfePoolMemoryAlign(sizeof(char) * 256, wfeAlignOf(char)); // Should be stored on 1KB instead 512B

    mu_assert("initialize pool", wfePoolInit(&pool) == WFE_SUCCESS);
    wfeData *d1 = wfePoolGet(&pool, s1, wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d1 request)", d1 != NULL);
    mu_assert("did not initialize tiny tier", pool.tiny != NULL);
    mu_assert("tiny data is not allocated within tiny tier", d1 >= pool.tiny->current->start && d1 < pool.tiny->current->end );

    wfeData *d2 = wfePoolGet(&pool, s2, wfeAlignOf(char));
    mu_assert("unexpected null pointer (d2 request)", d2 != NULL);
    mu_assert("did not initialize small tier", pool.small != NULL);
    mu_assert("small data is not allocated within small tier", d2 >= pool.small->current->start && d2 < pool.small->current->end );

    wfePoolFinalize(&pool);
    return 0;
}
static char * test_pool_recycle() {
    wfePool pool;
    wfeData *d1 = NULL, *d2 = NULL, *d3 = NULL, *d4 = NULL;
    wfeSize size = wfePoolMemoryAlign(sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));

    mu_assert("initialize tier", wfePoolInit(&pool) == WFE_SUCCESS);

    d1 = wfePoolGet(&pool, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d1 request)", d1 != NULL);
    mu_assert("data outside of current chunk space (first request)", d1 >= pool.tiny->current->start && d1 < pool.tiny->current->end);

    d2 = wfePoolGet(&pool, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d2 request)", d2 != NULL);
    mu_assert("data outside of current chunk space (second request)", d2 >= pool.tiny->current->start && d2 < pool.tiny->current->end);

    wfePoolRecycle(&pool);
    d3 = wfePoolGet(&pool, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d3 request)", d3 != NULL);
    mu_assert("did not recycle d1", d1 == d3);

    d4 = wfePoolGet(&pool, sizeof(wfePoolDummy), wfeAlignOf(wfePoolDummy));
    mu_assert("unexpected null pointer (d3 request)", d4 != NULL);
    mu_assert("did not recycle d2", d2 == d4);

    wfePoolFinalize(&pool);
    return 0;
}

static char * pool_suite() {
    mu_suite_start(pool);
    mu_run_test(test_pool_block_init_finalize);
    mu_run_test(test_pool_block_size);
    mu_run_test(test_pool_tier_init_finalize);
    mu_run_test(test_pool_tier_size);
    mu_run_test(test_pool_tier_get);
    mu_run_test(test_pool_tier_allocate_bigger);
    mu_run_test(test_pool_tier_recycle);
    mu_run_test(test_pool_init_finalize);
    mu_run_test(test_pool_misc_and_size);
    mu_run_test(test_pool_get);
    mu_run_test(test_pool_recycle);
    mu_suite_end(pool);
    return 0;
}

