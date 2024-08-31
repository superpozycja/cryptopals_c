#ifndef CRYPTANALYSIS_H
#define CRYPTANALYSIS_H

#include <ctype.h>
#include <string.h>

#include "ba.h"

/* define common cryptanalytic functionality */

unsigned long score_letter(char c);
unsigned long score_english(ba *text);
ba *decrypt_scxor(ba *ct);
void encrypt_rkxor(ba *pt, ba *key);

#endif
