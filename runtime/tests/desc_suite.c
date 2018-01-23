#include "minunit.h"
#include <wfe/desc.h>
#include <msgpack.h>
#include <string.h>

void init_dummy_desc(wfeDesc* desc, msgpack_sbuffer* sbuf) {
    msgpack_packer pk;
    msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);

    // will insert nkeys pairs.
    const int nkeys = 3;
    msgpack_pack_map(&pk, nkeys);

    // first pair (int)
    const char *kint = "myint";
    const int vint = 3241;
    msgpack_pack_str(&pk, strlen(kint));
    msgpack_pack_str_body(&pk, kint, strlen(kint));
    msgpack_pack_int(&pk, vint);

    // second pair (double)
    const char *kdouble = "mydouble";
    const float vdouble = 568.4;
    msgpack_pack_str(&pk, strlen(kdouble));
    msgpack_pack_str_body(&pk, kdouble, strlen(kdouble));
    msgpack_pack_int(&pk, vdouble);

    // third pair (string)
    const char *kstr = "mystring";
    const char *vstr = "hello world";
    msgpack_pack_str(&pk, strlen(kstr));
    msgpack_pack_str_body(&pk, kstr, strlen(kstr));
    msgpack_pack_str(&pk, strlen(vstr));
    msgpack_pack_str_body(&pk, vstr, strlen(vstr));

    desc->srcbuf = sbuf->data;
    desc->srcsize = sbuf->size;
}

static char * test_desc_init_finalize() {
    wfeDesc desc;
    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    mu_assert("desc srcbuf is not null", desc.srcbuf == NULL);
    mu_assert("desc srcsize is not zero", desc.srcsize == 0L);
    wfeDescFinalize(&desc);
    return 0;
}

static char * test_desc_next_key() {
    wfeDesc desc;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    init_dummy_desc(&desc, &sbuf);

    // destroy remaining data.
    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&sbuf);
    return 0;
}

static char * desc_suite() {
    mu_suite_start(desc);
    mu_run_test(test_desc_init_finalize);
    mu_suite_end(desc);
    return 0;
}

