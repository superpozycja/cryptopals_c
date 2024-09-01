#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int hex_to_base64(const char* hex, char** b64)
{
	const char b64table[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int leftover_length;
	int block_count;
	int res_length;
	int length;
	char *res;
	int i;

	length = strlen(hex);
	block_count = length / 6;
	leftover_length = length % 6;
	res_length = block_count * 4;

	if (leftover_length%2 == 1) {
		fprintf(stderr, "bad sequence length\n");
		return -1;
	}

	res = (char *) malloc(sizeof(char) * res_length + 2);

	for (i = 0; i < block_count + (leftover_length != 0); i++) {
		char block[6+1];
		int block_hex;
		int end_ix;
		int j;

		end_ix = i < block_count ? 6 : leftover_length;
		memcpy(block, hex+(6*i), end_ix);
		block[end_ix] = '\0';

		block_hex = strtol(block, NULL, 16);
		block_hex <<= 4 * (6 - end_ix);

		for (j = 0; j < 4; j++) {
			char b64;

			b64 = b64table[(block_hex >> 6 * (3 - j)) & 0x3f];
			res[4 *i + j] = j <= end_ix / 2 ? b64 : '=';
		}
	}

	*b64 = res;
	return 0;
}

int main(int argc, char* argv[]) 
{
	char* b64;
	int ret;

	if (hex_to_base64(argv[1], &b64) == 0)
		printf("%s\n", b64);

	return 0;
}
