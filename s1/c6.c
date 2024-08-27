#include <stdio.h>
#include <ctype.h>
#include <ba.h>
#include <cryptanalysis.h>

const char b64table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64_chunk_to_hex(char **hex, char *b64)
{
	unsigned int val;
	unsigned int sz;
	unsigned int i;

	if (strlen(b64) != 4)
		return -1;

	val = 0;
	for (i = 0; i < 4; i++) {
		int j = 0;
		val <<= 6;

		while (b64[i] != '=' && b64table[j] != b64[i])
			j++;

		val += j;
	}

	sz = 6 - 2 * ((b64[2] == '=') + (b64[3] == '='));
	sz++;

	*hex = (char *) malloc(sizeof(char) * sz);
	snprintf(*hex, sz, "%0*x", sz - 1, val);
	return 0;
}

int base64_to_hex(char **hex, char *b64)
{
	unsigned int len;

	len = (strlen(b64) * 3 / 4) + 2;
	*hex = (char *) malloc(sizeof(char *) * len);

	for (int i = 0; i < strlen(b64)/4; i++) {
		char b64_chunk[5];
		char *hex_chunk;

		b64_chunk[4] = '\0';
		memcpy(b64_chunk, b64+4*i, 4);
		b64_chunk_to_hex(&hex_chunk, b64_chunk);
		strcat(*hex, hex_chunk);
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
	ba_fprint(ct, stdout, 0);
	printf("\n");

	fclose(f);
}
