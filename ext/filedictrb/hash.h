#ifndef HASH_H
#define HASH_H 1

#include "filedictrb.h"

typedef struct fd_hash_t {
    filedict_t filedict;
} fd_hash_t;

void fdrb_init_hash(void);

#endif /* HASH_H */
