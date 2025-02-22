#ifndef UTIL_H_1e491544
#define UTIL_H_1e491544

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* utility funcs */

int hex_to_base64(char **b64, char *hex);
int base64_to_hex(char **hex, char *b64);

#endif
