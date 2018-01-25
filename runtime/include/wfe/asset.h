#ifndef WFE_ASSET_H
#define WFE_ASSET_H
#include <wfe/types.h>
#include <wfe/pool.h>
#include <wfe/desc.h>

#define WFE_FILE_SEPARATOR ('/')
#define WFE_FILE_SEPARATOR_STR ("/")
#define WFE_ASSET_FILE_ACCESS_ERROR WFE_MAKE_FILE_ERROR(50)
#define WFE_DID_NOT_READ_ALL_FILE WFE_MAKE_FILE_ERROR(51)

/**
 * Sets the search path for all asset loading.
 *
 * Warning: no thread-safe, call only at begining.
 * Params:
 *  - searchPath to assign.
 */
void wfeAssetSetSearchPath(wfeChar *searchPath);

/**
 * Loads a raw asset from disc to memory as buffer on a pool.
 *
 * Params:
 *  - name and folder of asset
 *  - ext for extension of asset
 *  - pool to allocate memory
 *  - data (out) reference to asset start.
 *  - size (out) size of data.
 *
 * Return:
 *  - WFE_SUCCESS if asset could be loaded.
 *  - WFE_ASSET_OMEM if there are no memory for asset.
 *  - WFE_POOL_* in case that pool returns error.
 *  - WFE_ASSET_FILE_ACCESS_ERROR if file does not exists or is unable to read.
 *  - WFE_DID_NOT_READ_ALL_FILE if read bytes are not equal than total file size.
 */
wfeError wfeAssetLoadRaw(const wfeChar *name, const wfeChar *ext, wfePool *pool, const wfeData **data, wfeSize *size);

/**
 * Loads description asset, used as replacement for JSON and XML.
 *
 * Params:
 *  - name of description, without extension.
 *  - pool to allocate memory.
 *  - desc (out) to put data from asset.
 *
 * Return:
 *  - WFE_SUCCESS if desc could be loaded.
 *  - All errors from wfeLoadRaw.
 */
wfeError wfeAssetLoadDesc(const wfeChar *name, wfePool *pool, wfeDesc *desc);

#endif /* WFE_ASSET_H */
