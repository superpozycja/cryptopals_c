#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ba.h>
#include <util.h>

int main(int argc, char *argv[])
{
	unsigned char *key = (unsigned char*) "YELLOW SUBMARINE";
	char line[256];
	int pt_len;
	char *b64;
	char *hex;
	char *pt;
	FILE *f;
	ba *ct;
	int sz;

	b64 = (char *) malloc(sizeof(char));

	f = fopen(argv[1], "r");
	sz = 0;
	while (fgets(line, sizeof(line), f)) {
		sz += strlen(line) - 1;
		b64 = (char *) realloc(b64, sz);
		strncat(b64, line, strlen(line) - 1);
	}

	base64_to_hex(&hex, b64);
	printf("%s\n", hex);
	return 0;
}
