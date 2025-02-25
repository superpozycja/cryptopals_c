#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ba.h>
#include <util.h>
#include <aes.h>
#include <block.h>

int main(int argc, char *argv[])
{
	char *key = "YELLOW SUBMARINE";
	char line[256];
	int pt_len;
	char *b64;
	char *hex;
	FILE *f;
	ba *key_ba;
	ba *ct;
	ba *pt;
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

	ct = ba_from_hex(hex);
	key_ba = ba_from_string(key);

	decrypt_ecb(ct, key_ba, &pt, 16, aes_128_decrypt);

	printf("decrypted successfully\n");
	ba_fprint_ascii(pt, stdout, 0);
	printf("\n");

	return 0;
}
