#include "filedictrb.h"

VALUE rb_mFiledictrb;

void
Init_filedictrb(void)
{
  rb_mFiledictrb = rb_define_module("Filedictrb");
}
