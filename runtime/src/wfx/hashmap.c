#include <wfx/hashmap.h>
#include <wfe/types.h>
#include <stdlib.h>
#include <string.h>
#include <zlib/zlib.h> /* for crc32 */

#define INITIAL_SIZE (256)
#define MAX_CHAIN_LENGTH (8)

/*
 * Calculate best position in hasmap's data.
 *
 * Params:
 *  - hashmap owner of data.
 *  - key for index calculation.
 *  - (out) index of location.
 * Return:
 *  - WFE_SUCCESS when key is successfully hashed into map.
 *  - WFE_HASHMAP_FULL when hashmap has no available slots.
 */
wfeError wfeHashmapHash(wfeHashmap* hashmap, const wfeData* key, wfeSize *index);

/**
 * Makes an index from a key string.
 *
 * Params:
 *  - hashmap where data will be stored.
 *  - keystring to be transformed to index.
 * Returns:
 *  - an index within table of hashmap.
 */
wfeSize wfeHashmapMakeIndex(wfeHashmap* hashmap, const wfeData* keystring);

/**
 * Doubles the size of the hashmap, and rehashes all the elements.
 *
 * Params:
 *  - hashmap to resize and re-map.
 * Returns:
 *  - WFE_SUCCESS if map is resized.
 *  - WFE_HASHMAP_OMEM_REHASH if no memory is available for re-hash.
 */
wfeError wfeHashmapRehash(wfeHashmap* hashmap);

wfeError wfeHashmapInit(wfeHashmap* hashmap) {
    wfeError status = WFE_SUCCESS;
    hashmap->data = NULL;
    hashmap->size = 0;
    hashmap->tablesize = 0;

    hashmap->data = calloc(INITIAL_SIZE, sizeof(wfeHashmapElement));
    if (hashmap->data == NULL) {
        status = WFE_HASHMAP_OMEM_ELEMENT; goto finalize;
    }

    hashmap->tablesize = INITIAL_SIZE;
    hashmap->size = 0;

finalize:
    if (WFE_HAS_FAILED(status))
        wfeHashmapFinalize(hashmap);

    return status;
}

void wfeHashmapDestroy(wfeHashmap* hashmap) {
    if (hashmap->data != NULL) {
        free(hashmap->data);
        hashmap->data = NULL;
    }
}

wfeSize wfeHashmapLength(wfeHashmap* hashmap) {
    return hashmap->size;
}


wfeError wfeHashmapIterate(wfeHashmap* hashmap, wfeHashmapIterator iter, wfeAny userdata) {
    wfeSize i;

    /* On empty hashmap, return immediately */
    if (wfeHashmapLength(hashmap) <= 0)
        return WFE_HASHMAP_MISSING;

    /* Linear probing */
    for(i = 0; i< hashmap->tablesize; i++) {
        if(hashmap->data[i].inuse != 0) {
            wfeAny data = (wfeAny) hashmap->data[i].data;
            wfeError status = iter(userdata, data);
            if (WFE_HAS_FAILED(status)) {
                return status;
            }
        }
    }

    return WFE_SUCCESS;
}

wfeError wfeHashmapPut(wfeHashmap* hashmap, const wfeData *key, wfeAny item) {
    wfeSize index;

    /* Find a place to put our value */
    wfeError status = wfeHashmapHash(hashmap, key, &index);
    while(status == WFE_HASHMAP_FULL){
        wfeError rhstatus = wfeHashmapRehash(hashmap);
        if (WFE_HAS_FAILED(rhstatus)) {
            return rhstatus;
        }

        status = wfeHashmapHash(hashmap, key, &index);
    }

    /* Set the data */
    hashmap->data[index].data = item;
    hashmap->data[index].key = key;
    hashmap->data[index].inuse = 1;
    hashmap->size++;

    return WFE_SUCCESS;
}

wfeError wfeHashmapGet(wfeHashmap* hashmap, const wfeData *key, wfeAny *arg) {
    wfeSize curr;
    int i;

    /* Find data location */
    curr = wfeHashmapMakeIndex(hashmap, key);

    /* Linear probing, if necessary */
    for(i = 0; i<MAX_CHAIN_LENGTH; i++) {
        int inuse = hashmap->data[curr].inuse;
        if (inuse == 1){
            if (strcmp(hashmap->data[curr].key,key)==0){
                *arg = (hashmap->data[curr].data);
                return WFE_SUCCESS;
            }
        }

        curr = (curr + 1) % hashmap->tablesize;
    }

    *arg = NULL;

    /* Not found */
    return WFE_HASHMAP_MISSING;
}

wfeError wfeHashmapRemove(wfeHashmap* hashmap, const wfeData *key) {
    int i;
    int curr;

    /* Find key */
    curr = wfeHashmapMakeIndex(hashmap, key);

    /* Linear probing, if necessary */
    for(i = 0; i<MAX_CHAIN_LENGTH; i++){

        int inuse = hashmap->data[curr].inuse;
        if (inuse == 1){
            if (strcmp(hashmap->data[curr].key,key)==0){
                /* Blank out the fields */
                hashmap->data[curr].inuse = 0;
                hashmap->data[curr].data = NULL;
                hashmap->data[curr].key = NULL;

                /* Reduce the size */
                hashmap->size--;
                return WFE_SUCCESS;
            }
        }
        curr = (curr + 1) % hashmap->tablesize;
    }

    /* Data not found */
    return WFE_HASHMAP_MISSING;
}

wfeError wfeHashmapHash(wfeHashmap* hashmap, const wfeData* key, wfeSize *index){
    int i;

    /* If full, return immediately */
    if(hashmap->size >= (hashmap->tablesize/2))
        return WFE_HASHMAP_FULL;

    /* Find the best index */
    *index = wfeHashmapMakeIndex(hashmap, key);

    /* Linear probing */
    for(i = 0; i< MAX_CHAIN_LENGTH; i++){
        if(hashmap->data[*index].inuse == 0)
            return WFE_SUCCESS;

        if(hashmap->data[*index].inuse == 1 && (strcmp(hashmap->data[*index].key,key)==0))
            return WFE_SUCCESS;

        *index = (*index + 1) % hashmap->tablesize;
    }

    return WFE_HASHMAP_FULL;
}

wfeSize wfeHashmapMakeIndex(wfeHashmap* hashmap, const wfeData* keystring){
    unsigned long crc = crc32(0L, Z_NULL, 0);
    unsigned long key = crc32_z(crc, (unsigned char*)(keystring), strlen(keystring));
    /* Robert Jenkins' 32 bit Mix Function */
    key += (key << 12);
    key ^= (key >> 22);
    key += (key << 4);
    key ^= (key >> 9);
    key += (key << 10);
    key ^= (key >> 2);
    key += (key << 7);
    key ^= (key >> 12);

    /* Knuth's Multiplicative Method */
    key = (key >> 3) * 2654435761;

    return key % hashmap->tablesize;
}

wfeError wfeHashmapRehash(wfeHashmap* hashmap) {
    int i;
    int old_size;
    wfeHashmapElement* curr;

    /* Setup the new elements */
    wfeHashmapElement* temp = (wfeHashmapElement *) calloc(2 * hashmap->tablesize, sizeof(wfeHashmapElement));
    if(!temp)
        return WFE_HASHMAP_OMEM_REHASH;

    /* Update the array */
    curr = hashmap->data;
    hashmap->data = temp;

    /* Update the size */
    old_size = hashmap->tablesize;
    hashmap->tablesize = 2 * hashmap->tablesize;
    hashmap->size = 0;

    /* Rehash the elements */
    for(i = 0; i < old_size; i++){
        int status;

        if (curr[i].inuse == 0)
            continue;

        status = wfeHashmapPut(hashmap, curr[i].key, curr[i].data);
        if (WFE_HAS_FAILED(status))
            return status;
    }

    free(curr);
    return WFE_SUCCESS;
}
