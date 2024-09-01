#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <ctype.h>
#include <string.h>

#include "ba.h"

/* define common cryptanalytic functionality */

unsigned long score_letter(char c);
unsigned long score_english(ba *text);
ba *decrypt_scxor(ba *ct);
void encrypt_rkxor(ba *pt, ba *key);
float index_of_coincidence(ba *ct);
unsigned int guess_vigenere_keylen(ba *ct);

#endif
