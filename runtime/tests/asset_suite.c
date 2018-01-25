#include "minunit.h"
#include <wfe/asset.h>
#include <wfe/pool.h>
#include <wfe/desc.h>
#include <string.h>

static char * test_asset_load_raw() {
    wfePool pool;
    wfeError code = WFE_SUCCESS;

    code = wfePoolInit(&pool);
    mu_assert("could not init pool", !WFE_HAVE_FAILED(code));

    const wfeChar *data;
    wfeSize size;

    code = wfeAssetLoadRaw("test_asset_load_raw", ".txt", &pool, &data, &size);
    mu_assert("could not load test_asset_load_raw.txt", !WFE_HAVE_FAILED(code));
    mu_assert("did not read any data from asset", size > 0L);
    mu_assert("wrong data from asset", strncmp(data, "this is plain text", size));

    wfePoolFinalize(&pool);
    return 0;
}

static char * test_asset_load_desc() {
    wfePool pool;
    wfeError code = WFE_SUCCESS;

    code = wfePoolInit(&pool);
    mu_assert("could not init pool", !WFE_HAVE_FAILED(code));

    wfeDesc desc;
    code = wfeDescInit(&desc);
    mu_assert("could not init desc", !WFE_HAVE_FAILED(code));

    code = wfeAssetLoadDesc("test_asset_load_desc", &pool, &desc);
    mu_assert("could not load test_asset_load_desc.desc", !WFE_HAVE_FAILED(code));

    const wfeData *key = NULL;
    wfeSize ksize = 0L;
    code = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access first key of desc", !WFE_HAVE_FAILED(code));
    mu_assert("unexpected key name", strncmp("int", key, ksize) == 0
            || strncmp("num", key, ksize) == 0
            || strncmp("str", key, ksize) == 0);

    wfeDescFinalize(&desc);
    wfePoolFinalize(&pool);
    return 0;
}

static char * asset_suite() {
    char *envsp = getenv("WFE_SEARCH_PATH");
    if (envsp != NULL)
        wfeAssetSetSearchPath(envsp);
    else
        wfeAssetSetSearchPath("tests/assets");

    mu_suite_start(asset);
    mu_run_test(test_asset_load_raw);
    mu_run_test(test_asset_load_desc);
    mu_suite_end(asset);
    return 0;
}

