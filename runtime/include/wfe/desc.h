#ifndef WFE_DESC_H
#define WFE_DESC_H
#include <wfe/types.h>
#include <msgpack.h>

#define WFE_DESC_UNSUPPORTED_TYPE WFE_MAKE_API_ERROR(40)
#define WFE_DESC_MSGPACK_ERROR WFE_MAKE_API_ERROR(41)
#define WFE_DESC_NULL_VALUE WFE_MAKE_API_ERROR(42)

/**
 * Property descriptors for runtime, commonly used as a
 * simple and fast replacement for JSON or XML to configuration
 * and object descriptions.
 *
 */
typedef struct wfeDesc {
    wfeBool haveMap;
    msgpack_unpacked result;
    msgpack_object_map map;

    wfeInt32 currentKey;
    msgpack_object currentVal;
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
 * Decodes a msgpack map from a raw buffer, getting .
 * Params:
 *  - desc to link buffer.
 *  - buf of bytes with msgpack encoded data.
 *  - len of buffer.
 * Return:
 *  - WFE_SUCCESS when map have been loaded.
 *  - WFE_DESC_UNSUPPORTED_TYPE if first type on msgpack is not a map.
 *  - WFE_DESC_MAGPACK_ERROR when msgpack returns an error.
 */
wfeError wfeDescDecodeBuffer(wfeDesc *desc, wfeData *buf, wfeSize len);

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
 *  - keystr (out) a non-null terminated string containing the next available key.
 *  - keysize (out) size of key string.
 * Return:
 *  - WFE_DONE (WFE_SUCCESS) if no more keys are available, check with WFE_SHOULD_CONTINUE.
 *  - WFE_CONTINUE if key have been decoded and there are more keys, check with WFE_SHOULD_CONTINUE.
 *  - WFE_DESC_UNSUPPORTED_TYPE if key is not an string, same behaviour as WFE_SHOULD_CONTINUE.
 */
wfeError wfeDescNextKey(wfeDesc *desc, const wfeChar **keystr, wfeSize *const keysize);

/**
 * Reads current value as integer.
 *
 * Params:
 *  - desc containing value.
 *  - value (out) to put current value as integer.
 *
 * Returns:
 *  - WFE_SUCCESS if value has been successfully copied.
 *  - WFE_DESC_UNSUPPORTED_TYPE if current values it not an integer.
 *  - WFE_DESC_NULL_VALUE if current value is null.
 */
wfeError wfeDescGetInt(wfeDesc *desc, wfeInt *value);

/**
 * Reads current value as num (double).
 *
 * Params:
 *  - desc containing value.
 *  - value (out) to put current value as double.
 *
 * Returns:
 *  - WFE_SUCCESS if value has been successfully copied.
 *  - WFE_DESC_UNSUPPORTED_TYPE if current values it not an double or floating point type.
 *  - WFE_DESC_NULL_VALUE if current value is null.
 */
wfeError wfeDescGetNum(wfeDesc *desc, wfeNum *value);

/**
 * Reads current value as string.
 *
 * Warning: strings will be not perserved after calling wfeDescFinalize.
 * Params:
 *  - desc containing value.
 *  - value (out) to put current value as string (not null-terminated).
 *  - size (out) size of string.
 *
 * Returns:
 *  - WFE_SUCCESS if value has been successfully copied.
 *  - WFE_DESC_UNSUPPORTED_TYPE if current values it not an string.
 *  - WFE_DESC_NULL_VALUE if current value is null.
 */
wfeError wfeDescGetString(wfeDesc *desc, const wfeData **value, wfeSize *const size);

#endif /* WFE_DESC_H */
