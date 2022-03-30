#include "filedictrb.h"
#include "hash.h"

VALUE mFiledict;

void
Init_filedictrb(void)
{
    mFiledict = rb_define_module("Filedict");
    fdrb_init_hash();
}
