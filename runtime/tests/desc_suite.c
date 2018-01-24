#include "minunit.h"
#include <lmath/mathutil.h>
#include <wfe/desc.h>
#include <msgpack.h>
#include <string.h>

// populates a valid dummy object for desc
void populate_sbuffer_valid(msgpack_sbuffer* sbuf);

// populates an valid dummy object for desc, this version does not have a map as first element.
void populate_sbuffer_invalid0(msgpack_sbuffer* sbuf);

// populates an valid dummy object for desc, this version holds unsupported types.
void populate_sbuffer_invalid1(msgpack_sbuffer* sbuf);

static char * test_desc_init_finalize() {
    wfeDesc desc;
    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    mu_assert("haveMap marked as true when is expected to be false", desc.haveMap == WFE_FALSE);
    mu_assert("unpacked zone should be null at initialization", desc.result.zone == NULL);
    wfeDescFinalize(&desc);
    return 0;
}

static char * test_desc_decode_buffer() {
    wfeDesc desc;
    msgpack_sbuffer okbuf;
    msgpack_sbuffer_init(&okbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    populate_sbuffer_valid(&okbuf);

    mu_assert("failed buffer decode", !WFE_HAVE_FAILED(wfeDescDecodeBuffer(&desc, okbuf.data, okbuf.size)));
    mu_assert("haveMap is makerd as false when its expected to be true", desc.haveMap == WFE_TRUE);
    mu_assert("no zone have been allocated for object storage", desc.result.zone != NULL);

    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&okbuf);
    return 0;
}

static char * test_desc_decode_buffer_nomap() {
    wfeDesc desc;
    msgpack_sbuffer wrongbuf;
    msgpack_sbuffer_init(&wrongbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    populate_sbuffer_invalid0(&wrongbuf);

    mu_assert("no failure reported after decoding a bad buffer", WFE_HAVE_FAILED(wfeDescDecodeBuffer(&desc, wrongbuf.data, wrongbuf.size)));
    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&wrongbuf);
    return 0;
}

static char * test_desc_next_key() {
    wfeDesc desc;
    msgpack_sbuffer okbuf;
    msgpack_sbuffer_init(&okbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    populate_sbuffer_valid(&okbuf);
    mu_assert("failed buffer decode", !WFE_HAVE_FAILED(wfeDescDecodeBuffer(&desc, okbuf.data, okbuf.size)));

    const wfeData *key = NULL;
    wfeSize ksize = 0L;
    wfeError status = WFE_SUCCESS;

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access first key", !WFE_HAVE_FAILED(status));
    mu_assert("cannot continue after first call", WFE_SHOULD_CONTINUE(status));
    mu_assert("first key is not myint", strncmp("myint", key, ksize) == 0);

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access second key", !WFE_HAVE_FAILED(status));
    mu_assert("cannot continue after second call", WFE_SHOULD_CONTINUE(status));
    mu_assert("second key is not mydouble", strncmp("mydouble", key, ksize) == 0);

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access third key", !WFE_HAVE_FAILED(status));
    mu_assert("cannot continue after third call", WFE_SHOULD_CONTINUE(status));
    mu_assert("thrid key is not mystring", strncmp("mystring", key, ksize) == 0);

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("can continue after fourd call", !WFE_SHOULD_CONTINUE(status));

    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&okbuf);
    return 0;
}

static char * test_desc_get_int() {
    wfeDesc desc;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    populate_sbuffer_valid(&sbuf);
    mu_assert("failed buffer decode", !WFE_HAVE_FAILED(wfeDescDecodeBuffer(&desc, sbuf.data, sbuf.size)));

    const wfeData *key = NULL;
    wfeSize ksize = 0L;
    wfeError status = WFE_SUCCESS;

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access first key", !WFE_HAVE_FAILED(status));
    mu_assert("cannot continue after first call", WFE_SHOULD_CONTINUE(status));
    mu_assert("first key is not myint", strncmp("myint", key, ksize) == 0);

    wfeInt value = 0;
    status = wfeDescGetInt(&desc, &value);
    mu_assert("cannot read int value", !WFE_HAVE_FAILED(status));
    mu_assert("read value does not match with populated value", 3241 == value);

    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&sbuf);
    return 0;
}

static char * test_desc_get_num() {
    wfeDesc desc;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    populate_sbuffer_valid(&sbuf);
    mu_assert("failed buffer decode", !WFE_HAVE_FAILED(wfeDescDecodeBuffer(&desc, sbuf.data, sbuf.size)));

    const wfeData *key = NULL;
    wfeSize ksize = 0L;
    wfeError status = WFE_SUCCESS;

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access first key", !WFE_HAVE_FAILED(status));

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access second key", !WFE_HAVE_FAILED(status));

    wfeNum value = 0.0;
    status = wfeDescGetNum(&desc, &value);
    mu_assert("cannot read num value", !WFE_HAVE_FAILED(status));
    mu_assert("read value does not match with populated value (aprox)", APPROXEQ(568.4, value));

    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&sbuf);
    return 0;
}

static char * test_desc_get_str() {
    wfeDesc desc;
    msgpack_sbuffer sbuf;
    msgpack_sbuffer_init(&sbuf);

    mu_assert("could not initialize desc", !WFE_HAVE_FAILED(wfeDescInit(&desc)));
    populate_sbuffer_valid(&sbuf);
    mu_assert("failed buffer decode", !WFE_HAVE_FAILED(wfeDescDecodeBuffer(&desc, sbuf.data, sbuf.size)));

    const wfeData *key = NULL;
    wfeSize ksize = 0L;
    wfeError status = WFE_SUCCESS;

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access first key", !WFE_HAVE_FAILED(status));

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access second key", !WFE_HAVE_FAILED(status));

    status = wfeDescNextKey(&desc, &key, &ksize);
    mu_assert("cannot access third key", !WFE_HAVE_FAILED(status));

    const wfeChar *value = NULL;
    wfeSize vsize = 0L;
    status = wfeDescGetString(&desc, &value, &vsize);
    mu_assert("cannot read string value", !WFE_HAVE_FAILED(status));
    mu_assert("incorrect size of read string", vsize == strlen("hello world"));
    mu_assert("read value does not match with populated value", strncmp("hello world", value, vsize) == 0);

    wfeDescFinalize(&desc);
    msgpack_sbuffer_destroy(&sbuf);
    return 0;
}

static char * desc_suite() {
    mu_suite_start(desc);
    mu_run_test(test_desc_init_finalize);
    mu_run_test(test_desc_decode_buffer);
    mu_run_test(test_desc_decode_buffer_nomap);
    mu_run_test(test_desc_next_key);
    mu_run_test(test_desc_get_int);
    mu_run_test(test_desc_get_num);
    mu_run_test(test_desc_get_str);
    mu_suite_end(desc);
    return 0;
}

void populate_sbuffer_valid(msgpack_sbuffer* sbuf) {
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
    msgpack_pack_double(&pk, vdouble);

    // third pair (string)
    const char *kstr = "mystring";
    const char *vstr = "hello world";
    msgpack_pack_str(&pk, strlen(kstr));
    msgpack_pack_str_body(&pk, kstr, strlen(kstr));
    msgpack_pack_str(&pk, strlen(vstr));
    msgpack_pack_str_body(&pk, vstr, strlen(vstr));
}

void populate_sbuffer_invalid0(msgpack_sbuffer* sbuf) {
    msgpack_packer pk;
    msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);
    const int value = 4523;
    msgpack_pack_int(&pk, value);
}

