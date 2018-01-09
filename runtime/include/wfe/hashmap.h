#ifndef WFE_HASHMAP_H
#define WFE_HASHMAP_H
#include <stdio.h>
#include <wfe/types.h>

#define WFE_HASHMAP_MISSING WFE_MAKE_FAILURE(26)
#define WFE_HASHMAP_FULL WFE_MAKE_FAILURE(25)
#define WFE_HASHMAP_OMEM_ELEMENT WFE_MAKE_MEMORY_ERROR(21)
#define WFE_HASHMAP_OMEM_REHASH WFE_MAKE_MEMORY_ERROR(22)

/**
 * Iterator callback.
 *
 * This callback must return a map status code, if it returns anything
 * other than WFE_HASHMAP_OK the traversal is terminated. Also, it's highly
 * recommended to not reenter any Hashmap functions, or deadlock may arise.
 *
 * Prototype params:
 *  - (1) wfeAny userdata
 *  - (2) wfeAny current item
 *
 * Should return:
 *  - WFE_SUCCESS to continue.
 *  - WFE_FAILURE to interrupt and bubble error.
 */
typedef wfeError (*wfeHashmapIterator)(wfeAny, wfeAny);

/* We need to keep keys and values */
typedef struct _wfeHashmapElement {
    const wfeData* key;
    wfeData *data;
    wfeSize inuse;
} wfeHashmapElement;

/**
 * Hashmap implementation.
 *
 * Hashmap, implementation borrowed (re-written and sighlty modified for code style propouses)
 * from https://github.com/petewarden/c_hashmap.
 */
typedef struct _wfeHashmap {
    wfeSize tablesize;
    wfeSize size;
    wfeHashmapElement *data;
} wfeHashmap;

/**
 * Inits an wfeHashmap.
 * Params:
 *  - hashmap that is going to be inited.
 * Returns:
 *  - WFE_SUCCESS.
 *  - WFE_HASHMAP_OMEM_ELEMENT if no memory is available for handler.
 */
wfeError wfeHashmapInit(wfeHashmap* hashmap);

/**
 * Releases resources of a hashmap.
 *
 * Params:
 *  - hashmap to destroy.
 */
void wfeHashmapFinalize(wfeHashmap* hashmap);

/**
 * Get the current size of a hashmap.
 *
 * Params:
 *  - hashmap to know the size.
 * Return:
 *  - count of elements.
 */
wfeSize wfeHashmapLength(wfeHashmap* hashmap);

/**
 * Interate over items of a hashmap.
 *
 * Call iter param with userdata and itemdata as argument for each element in hashmap.
 *
 * Params:
 *  - hashmap to iterate.
 *  - iter function to callback for each item.
 *  - userdata to add as first argument to callback.
 *
 * Returns:
 *  - WFE_SUCCESS after complete iteration.
 *  - WFE_HASHMAP_MISSING when there are no keys to iterate.
 *  - WFE_FAILURE same of callback return if errors.
 */
wfeError wfeHashmapIterate(wfeHashmap* hashmap, wfeHashmapIterator iter, wfeAny userdata);

/**
 * Associate a string key with a pointer in the hashmap.
 * Params:
 *  - hashmap to put the association.
 *  - key of association.
 *  - item of association (pointer).
 * Return:
 *  - WFE_SUCCESS when association of key and item have been made into hashmap.
 *  - WFE_HASHMAP_OMEM_REHASH if no memory is available for rehash.
 */
wfeError wfeHashmapPut(wfeHashmap* hashmap, const wfeData *key, wfeAny item);

/**
 * Fetch an item from a hashmap using a key.
 *
 * Params:
 *  - hashmap to look for key.
 *  - key of item.
 *  - (out) the initial stored reference.
 * Return:
 *  - WFE_SUCCESS when item found and assigned to out parameter.
 *  - WFE_HASHMAP_MISSING if item is not found.
 */
wfeError wfeHashmapGet(wfeHashmap* hashmap, const wfeData *key, wfeAny *item);

/**
 * Removes an item using the given key.
 *
 * Params:
 *  - hashmap to look for key.
 *  - key of item.
 *
 * Return:
 *  - WFE_SUCCESS if item was found and deleted.
 *  - WFE_HASHMAP_MISSING if item is not found.
 */
wfeError wfeHashmapRemove(wfeHashmap* hashmap, const wfeData *key);

#endif /* WFE_HASHMAP_H */
