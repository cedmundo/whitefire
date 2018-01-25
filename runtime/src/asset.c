#include <wfe/asset.h>
#include <wfe/types.h>
#include <wfe/desc.h>
#include <wfe/pool.h>
#include <string.h>
#include <stdio.h>

static wfeChar *wfeSearchPath;
wfeChar *makePath(const wfeChar *name, const wfeChar *ext, wfePool *pool);

void wfeAssetSetSearchPath(wfeChar *searchPath) {
    wfeSearchPath = searchPath;
}

wfeError wfeAssetLoadRaw(name, ext, pool, data, size)
    const wfeChar *name;
    const wfeChar *ext;
    wfePool *pool;
    const wfeData **data;
    wfeSize *size;
{
    assert(wfeSearchPath != NULL /* Should initialize */);
    assert(name != NULL /* name should exists */);
    assert(ext != NULL /* ext should exists */);
    assert(pool != NULL /* memory should reference something */);
    assert(data != NULL /* Should reference something */);
    assert(size != NULL /* Should reference something */);

    *size = 0L;
    *data = NULL;
    wfeChar *fdata = NULL;
    wfeChar *fpath = makePath(name, ext, pool);
    if (fpath == NULL) {
        return pool->lastError;
    }

    FILE *file = fopen(fpath, "r");
    if (file == NULL) {
        return WFE_ASSET_FILE_ACCESS_ERROR;
    }

    fseek(file, 0L, SEEK_END);
    wfeSize fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    fdata = wfePoolGet(pool, fsize, wfeAlignOf(char));
    if (fdata == NULL) {
        fclose(file);
        return pool->lastError;
    }

    wfeSize rcount = fread(fdata, 1, fsize, file);
    if (rcount != fsize) {
        fclose(file);
        return WFE_DID_NOT_READ_ALL_FILE;
    }

    *size = fsize;
    *data = fdata;
    fclose(file);
    return WFE_SUCCESS;
}

wfeError wfeAssetLoadDesc(const wfeChar *name, wfePool *pool, wfeDesc *desc) {
    wfeError code = WFE_SUCCESS;
    const wfeData *data = NULL;
    wfeSize size = 0L;

    assert(name != NULL /* name should exists */);
    assert(desc != NULL /* desc should exists */);

    code = wfeAssetLoadRaw(name, ".desc", pool, &data, &size);
    if (WFE_HAVE_FAILED(code)) {
        return code;
    }

    code = wfeDescDecodeBuffer(desc, data, size);
    return code;
}

wfeChar *makePath(const wfeChar *name, const wfeChar *ext, wfePool *pool) {
    wfeSize splen = strlen(wfeSearchPath);
    wfeSize nalen = strlen(name);
    wfeSize exlen = strlen(ext);
    wfeSize fplen = splen + nalen + exlen + 2;
    wfeChar *fpath = wfePoolGet(pool, fplen, wfeAlignOf(char));
    if (fpath == NULL) {
        return NULL;
    }

    strcat(fpath, wfeSearchPath);
    if ( (nalen > 0 ? name[nalen] : 0) != WFE_FILE_SEPARATOR && (splen > 0 ? wfeSearchPath[splen-1] : 0) != WFE_FILE_SEPARATOR) {
        strcat(fpath, WFE_FILE_SEPARATOR_STR);
    }

    strcat(fpath, name);
    strcat(fpath, ext);
    return fpath;
}
