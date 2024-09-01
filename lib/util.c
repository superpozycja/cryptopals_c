#include "util.h"

const char b64table[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int b64_chunk_to_hex(char **hex, char *b64)
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

int hex_to_base64(char **b64, char *hex)
{
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
