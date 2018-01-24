#include <wfe/desc.h>
#include <msgpack.h>

wfeError wfeDescInit(wfeDesc *desc) {
    assert(desc != NULL /* desc should reference something */);

    desc->haveMap = WFE_FALSE;
    desc->result.zone = NULL;
    desc->currentKey = 0;
    return WFE_SUCCESS;
}

void wfeDescFinalize(wfeDesc *desc) {
    assert(desc != NULL /* desc should reference something */);

    if (WFE_TRUE == desc->haveMap) {
        msgpack_unpacked_destroy(&desc->result);
        desc->haveMap = WFE_FALSE;
    }
}

wfeError wfeDescDecodeBuffer(wfeDesc *desc, wfeData *buf, wfeSize len) {
    wfeSize offset = 0L;
    msgpack_unpack_return ret;

    assert(desc != NULL /* desc should reference something */);
    assert(buf != NULL /* buffer should contain data */);
    assert(len > 0L /* buffer should contain data */);

    msgpack_unpacked_init(&desc->result);
    desc->haveMap = WFE_TRUE;

    ret = msgpack_unpack_next(&desc->result, buf, len, &offset);
    if (ret == MSGPACK_UNPACK_SUCCESS) {
        msgpack_object obj = desc->result.data;
        if (obj.type != MSGPACK_OBJECT_MAP) {
            return WFE_DESC_UNSUPPORTED_TYPE;
        }

        desc->map = obj.via.map;
        return WFE_SUCCESS;
    }

    return WFE_DESC_MSGPACK_ERROR;
}

wfeError wfeDescNextKey(wfeDesc *desc, const wfeChar **keystr, wfeSize *const keysize){
    assert(desc != NULL /* desc should reference something */);
    assert(keystr != NULL /* keystr should reference shometing */);
    assert(keysize != NULL /* keylen should reference shometing */);

    msgpack_object_map map = desc->map;
    if (desc->currentKey >= map.size) {
        return WFE_DONE;
    }

    msgpack_object_kv pair = map.ptr[desc->currentKey];
    if (pair.key.type != MSGPACK_OBJECT_STR) {
        return WFE_DESC_UNSUPPORTED_TYPE;
    }

    // output key values.
    msgpack_object key = pair.key;
    *keysize = key.via.str.size;
    *keystr = key.via.str.ptr;

    // assign current value and move to next key.
    desc->currentVal = pair.val;
    desc->currentKey ++;
    return WFE_CONTINUE;
}

wfeError wfeDescGetInt(wfeDesc *desc, wfeInt *value) {
    assert(desc != NULL /* desc should reference something */);
    assert(value != NULL /* target value must point to something */);
    msgpack_object val = desc->currentVal;
    if (val.type == MSGPACK_OBJECT_NIL) {
        return WFE_DESC_NULL_VALUE;
    }

    if (val.type != MSGPACK_OBJECT_NEGATIVE_INTEGER
            && val.type != MSGPACK_OBJECT_POSITIVE_INTEGER
            && val.type != MSGPACK_OBJECT_BOOLEAN) {
        return WFE_DESC_UNSUPPORTED_TYPE;
    }

    *value = val.via.i64;
    return WFE_SUCCESS;
}

wfeError wfeDescGetNum(wfeDesc *desc, wfeNum *value) {
    assert(desc != NULL /* desc should reference something */);
    assert(value != NULL /* target value must point to something */);
    msgpack_object val = desc->currentVal;
    if (val.type == MSGPACK_OBJECT_NIL) {
        return WFE_DESC_NULL_VALUE;
    }

    if (val.type != MSGPACK_OBJECT_FLOAT32
            && val.type != MSGPACK_OBJECT_FLOAT64) {
        return WFE_DESC_UNSUPPORTED_TYPE;
    }

    *value = val.via.f64;
    return WFE_SUCCESS;
}

wfeError wfeDescGetString(wfeDesc *desc, const wfeData **value, wfeSize *const size) {
    assert(desc != NULL /* desc should reference something */);
    assert(value != NULL /* target value must point to something */);
    msgpack_object val = desc->currentVal;
    if (val.type == MSGPACK_OBJECT_NIL) {
        return WFE_DESC_NULL_VALUE;
    }

    if (val.type != MSGPACK_OBJECT_STR) {
        return WFE_DESC_UNSUPPORTED_TYPE;
    }

    *size = val.via.str.size;
    *value = val.via.str.ptr;
    return WFE_SUCCESS;
}
