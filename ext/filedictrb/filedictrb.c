#include "filedictrb.h"
#include "hash.h"

VALUE mFiledict;

void
Init_filedict(void)
{
    mFiledict = rb_define_module("Filedict");
    fdrb_init_hash();
}
