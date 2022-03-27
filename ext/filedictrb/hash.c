#include "hash.h"
#include "../filedict/filedict.h"

extern VALUE mFiledict;
VALUE cHash;

void fdrb_init_hash() {
    cHash = rb_define_class_under(mFiledict, "Hash", rb_cObject);
    // rb_define_alloc_func(cHash, trace_allocate);

    // rb_define_method(cHash, "initialize", trace_initialize, 2);
    // rb_define_method(cHash, "tracepoint", trace_tracepoint, 0);
}
