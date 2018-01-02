#include "minunit.h"
#include <wfe/pool.h>

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
    wfeSize size = sizeof(char) * 4;

    mu_assert("initialize tier", wfePoolTierInit(&tier, size) == WFE_SUCCESS);
    mu_assert("invalid first block", tier.first != NULL);
    mu_assert("invalid current block", tier.first == tier.current);
    mu_assert("invalid size of first block", wfePoolBlockTotalSize(tier.first) == size);

    wfePoolTierFinalize(&tier);
    mu_assert("should not reference memory after finalize", tier.first == NULL && tier.current == NULL);
    return 0;
}

static char * pool_suite() {
    mu_suite_start(pool);
    mu_run_test(test_pool_block_init_finalize);
    mu_run_test(test_pool_block_size);
    mu_run_test(test_pool_tier_init_finalize);
    mu_suite_end(pool);
    return 0;
}

