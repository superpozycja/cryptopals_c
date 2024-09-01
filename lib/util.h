#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* utility funcs */

int hex_to_base64(char **b64, char *hex);
int base64_to_hex(char **hex, char *b64);

#endif
