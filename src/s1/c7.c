#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ba.h>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

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

	sz = 3 - ((b64[2] == '=') + (b64[3] == '='));

	*hex = (char *) malloc(sizeof(char) * sz);
	i = 0;
	while (val > 0) {
		(*hex)[i] = val % 256;
		val /= 8;
		i++;
	}
	return 0;
}

int base64_to_hex(char **hex, char *b64)
{
	unsigned int len;

	len = (strlen(b64) * 3 / 4) + 1;
	*hex = (char *) malloc(sizeof(char *) * len);

	for (int i = 0; i < strlen(b64)/4; i++) {
		char b64_chunk[5];
		char *hex_chunk;

		b64_chunk[4] = '\0';
		memcpy(b64_chunk, b64+4*i, 4);
		b64_chunk_to_hex(&hex_chunk, b64_chunk);
		strcat(*hex, hex_chunk);
	}
	(*hex)[len-1] = '\0';
}

int decrypt(unsigned char *ct, int ct_len, unsigned char *key,
	    unsigned char *pt)
{
	EVP_CIPHER_CTX *ctx;
	int pt_len;
	int len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		return -1;

	if(EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1)
		return -1;

	if(EVP_DecryptUpdate(ctx, pt, &len, ct, ct_len) != 1)
		return -1;

	pt_len = len;

	if(EVP_DecryptFinal_ex(ctx, pt + len, &len) != 1)
		return -1;
	pt_len += len;

	EVP_CIPHER_CTX_free(ctx);
	return pt_len;
}

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
	
	pt = (char *) malloc(sizeof(char) * strlen(hex) + 1);

	pt_len = decrypt(hex, strlen(hex), key, pt);

	/* Add a NULL terminator. We are expecting printable text */
	//pt[pt_len] = '\0';
}
