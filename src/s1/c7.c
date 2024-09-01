#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ba.h>
#include <util.h>

/*
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
*/

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

	//pt_len = decrypt(hex, strlen(hex), key, pt);

	/* Add a NULL terminator. We are expecting printable text */
	//pt[pt_len] = '\0';
}
