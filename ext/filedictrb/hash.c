#include "hash.h"
#include "../filedict/filedict.h"

extern VALUE mFiledict;
VALUE cHash;

/*
 * ==============================
 * Custom allocation / GC support
 * ==============================
 *
 * Used as a reference:
 * https://github.com/ruby-prof/ruby-prof/blob/master/ext/ruby_prof/rp_profile.c
 *
 * This stuff is necessary in order for GC to work properly when we hold
 * references to Ruby objects in C code.
 */

static void fd_hash_mark(void *data) {
    /* TODO: do we need to hold ruby objects? */
    /* fd_hash_t *fd_hash = (fd_hash_t*)data; */
}

static void fd_hash_free(void *data) {
    fd_hash_t *fd_hash = (fd_hash_t*)data;

    filedict_deinit(&fd_hash->filedict);

    xfree(fd_hash);
}

size_t fd_hash_size(const void* _data) {
    return sizeof(fd_hash_t);
}

static const rb_data_type_t fd_hash_type =
{
    .wrap_struct_name = "Filedict::Hash",
    .function =
    {
        .dmark = fd_hash_mark,
        .dfree = fd_hash_free,
        .dsize = fd_hash_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE fd_hash_allocate(VALUE klass) {
    VALUE result;
    fd_hash_t* fd_hash;

    result = TypedData_Make_Struct(klass, fd_hash_t, &fd_hash_type, fd_hash);
    filedict_init(&fd_hash->filedict);

    return result;
}


/*
 * ==============================
 * Ruby method definitions
 * ==============================
 *
 */

static VALUE fd_hash_initialize(VALUE self, VALUE filename) {
    fd_hash_t *fd_hash = RTYPEDDATA_DATA(self);

    const char *filename_cstr = StringValuePtr(filename);
    filedict_open(&fd_hash->filedict, filename_cstr);

    if (fd_hash->filedict.error) {
        rb_raise(rb_eArgError, "Filedict error: %s", fd_hash->filedict.error);
    }

    return self;
}

void fdrb_init_hash() {
    cHash = rb_define_class_under(mFiledict, "Hash", rb_cObject);
    rb_define_alloc_func(cHash, fd_hash_allocate);

    rb_define_method(cHash, "initialize", fd_hash_initialize, 1);
}
