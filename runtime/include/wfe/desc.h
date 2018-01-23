#ifndef WFE_DESC_H
#define WFE_DESC_H
#include <wfe/types.h>
#include <msgpack.h>

/**
 * Property descriptors for runtime, commonly used as a
 * simple and fast replacement for JSON or XML to configuration
 * and object descriptions.
 *
 */
typedef struct wfeDesc {
    wfeData *srcbuf;
    wfeSize srcsize;
    wfeBool isUnpacked;
} wfeDesc;

/**
 * Initializes a desc structure with zero values. No memory is required at this point.
 * Params:
 *  - desc to be initialized.
 * Returns:
 *  - WFE_SUCCESS always.
 */
wfeError wfeDescInit(wfeDesc *desc);

/**
 * Finalizes a properties structure, removing msgpack memory zone.
 * Params:
 *  - desc to be finalized.
 */
void wfeDescFinalize(wfeDesc *desc);

/**
 * Looks for the next key on the description.
 *
 * If a buffer is provided and desc is not unpacked, then this function will try to
 * unpack first object on buffer and will expect that object is a map, otherwise an error
 * will be returned. Next calls will find the next key on the unpacked map.
 *
 * Warning: keys will be not perserved after calling wfeDescFinalize.
 * Params:
 *  - desc to iterate over.
 *  - key (out) a non-null terminated string containing the next available key.
 *  - keysize (out) size of key string.
 *
 */
wfeError wfeDescNextKey(wfeDesc *desc, wfeChar **key, wfeSize *keysize);

//wfeError wfeDescGetInt(wfeDesc *desc, wfeInt *value);
//wfeError wfeDescGetAny(wfeDesc *desc, wfeAny *value, wfeSize *size);
//wfeError wfeDescGetNum(wfeDesc *desc, wfeNum *value);
//wfeError wfeDescGetBool(wfeDesc *desc, wfeBool *value);
//wfeError wfeDescGetString(wfeDesc *desc, wfeData **value, wfeSize *size);

#endif /* WFE_DESC_H */
