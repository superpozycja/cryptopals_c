#ifndef AES_H_1e491544
#define AES_H_1e491544

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "ba.h"

int aes_128_encrypt(ba *plaintext, ba *key, ba **ciphertext);
int aes_128_decrypt(ba *ciphertext, ba *key, ba **plaintext);

int aes_192_encrypt(ba *plaintext, ba *key, ba **ciphertext);
int aes_192_decrypt(ba *ciphertext, ba *key, ba **plaintext);

int aes_256_encrypt(ba *plaintext, ba *key, ba **ciphertext);
int aes_256_decrypt(ba *ciphertext, ba *key, ba **plaintext);

#endif
