#include <wfe/desc.h>
#include <msgpack.h>

wfeError wfeDescInit(wfeDesc *desc) {
    desc->srcbuf = NULL;
    desc->srcsize = 0L;
    desc->isUnpacked = WFE_FALSE;
    return WFE_SUCCESS;
}

void wfeDescFinalize(wfeDesc *desc) {
    // TODO: release msgpack zone if required.
}
