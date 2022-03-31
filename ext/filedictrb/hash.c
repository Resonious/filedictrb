#include "hash.h"
#include "../filedict/filedict.h"

extern VALUE mFiledict;
VALUE cHash;
VALUE cSet;

ID id_add;
ID id_remove;
ID id_freeze;
ID id_fd_hash;
ID id_fd_key;

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

static VALUE fd_set_add(int argc, VALUE *argv, VALUE self) {
    VALUE fd_hash_ruby_object = rb_ivar_get(self, id_fd_hash);

    if (fd_hash_ruby_object == Qnil) {
        return rb_call_super(argc, argv);
    }

    fd_hash_t *fd_hash = RTYPEDDATA_DATA(fd_hash_ruby_object);
    int i;

    VALUE key = rb_ivar_get(self, id_fd_key);
    const char *key_cstr = StringValuePtr(key);
    const char *value_cstr;

    for (i = 0; i < argc; ++i) {
        value_cstr = StringValuePtr(argv[i]);
        filedict_insert_unique(&fd_hash->filedict, key_cstr, value_cstr);
    }

    return rb_call_super(argc, argv);
}

static VALUE fd_hash_access(VALUE self, VALUE key) {
    fd_hash_t *fd_hash = RTYPEDDATA_DATA(self);

    VALUE result = rb_class_new_instance(0, NULL, cSet);
    const char *key_cstr = StringValuePtr(key);

    filedict_read_t read = filedict_get(&fd_hash->filedict, key_cstr);

    int success = 1;
    while (success && read.value) {
        rb_funcall(result, id_add, 1, rb_str_new_cstr(read.value));
        success = filedict_get_next(&read);
    }

    rb_ivar_set(result, id_fd_hash, self);
    rb_ivar_set(result, id_fd_key, key);

    return result;
}

void fdrb_init_hash() {
    cHash = rb_define_class_under(mFiledict, "Hash", rb_cObject);
    rb_define_alloc_func(cHash, fd_hash_allocate);

    rb_define_method(cHash, "initialize", fd_hash_initialize, 1);
    rb_define_method(cHash, "at", fd_hash_access, 1);
    rb_define_method(cHash, "[]", fd_hash_access, 1);

    VALUE rb_cSet = rb_define_class("Set", rb_cObject);
    cSet = rb_define_class_under(mFiledict, "Set", rb_cSet);

    rb_define_method(cSet, "add", fd_set_add, -1);

    id_add = rb_intern("add");
    id_remove = rb_intern("remove");
    id_freeze = rb_intern("freeze");
    id_fd_hash = rb_intern_const("__fd_hash__");
    id_fd_key = rb_intern_const("__fd_hash_key__");
}
