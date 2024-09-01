#include <stdio.h>
#include <ctype.h>
#include <ba.h>
#include <analysis.h>
#include <util.h>

void break_w_keylen(ba *ct, unsigned int m)
{
	int i;

	for (i = 0; i < m; i++) {
		ba *chunk = (ba *) malloc(sizeof(ba));
		int j;
		int k;

		chunk->len = ct->len / m;
		chunk->val = (uint8_t *) malloc(sizeof(uint8_t) * chunk->len);
		
		for (j = 0, k = 0; j <chunk->len &&  k + i < ct->len; j++, k += m)
			chunk->val[j] = ct->val[k + i];

		chunk = decrypt_scxor(chunk);

		for (j = 0, k = 0; j <chunk->len &&  k + i < ct->len; j++, k += m)
			 ct->val[k + i] = chunk->val[j];
	}
}


/* use coincidence test to determine the key */
int main(int argc, char *argv[])
{
	char line[256];
	char *b64;
	char *hex;
	FILE *f;
	ba *ct;
	int sz;

	b64 = (char *) malloc(sizeof(char));

	f = fopen(argv[1], "r");
	if (!f)
		fprintf(stderr, "error opening file\n");

	sz = 1;
	while (fgets(line, sizeof(line), f)) {
		sz += strlen(line) - 1;
		b64 = (char *) realloc(b64, sz);
		strncat(b64, line, strlen(line) - 1);
	}

	base64_to_hex(&hex, b64);
	ct = ba_from_hex(hex);	
	
	int m = guess_vigenere_keylen(ct);

	break_w_keylen(ct, m);

	ba_fprint_ascii(ct, stdout, 0);
	printf("\n");

	ba_free(ct);
	fclose(f);
	return 0;
}
