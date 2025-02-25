#ifndef BLOCK_H_1e491544
#define BLOCK_H_1e491544

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "ba.h"

int encrypt_ecb(ba *plaintext, ba *key, ba **ciphertext, uint16_t block_size,
                int (* encrypt) (ba *, ba *, ba **));

int decrypt_ecb(ba *ciphertext, ba *key, ba **plaintext, uint16_t block_size,
                int (* decrypt) (ba *, ba *, ba **));

#endif
