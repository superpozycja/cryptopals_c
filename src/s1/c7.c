#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ba.h>
#include <util.h>
#include <aes.h>

int main(int argc, char *argv[])
{
	char *key = "YELLOW SUBMARINE";
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
	//printf("%s\n", hex);

	ba *pt_ba = ba_from_string("yoyoyoyoyoyoyoyo");
	ba *key_ba = ba_from_hex("2b7e151628aed2a6abf7158809cf4f3c");
	ba *key_192_ba = ba_from_hex("8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b");
	ba *ct_ba;

	aes_128_encrypt(pt_ba, key_ba, ct_ba);
	aes_192_encrypt(pt_ba, key_192_ba, ct_ba);

	return 0;
}
