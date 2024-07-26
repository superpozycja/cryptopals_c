#include <ba.h>
#include <stdlib.h>
#include <ctype.h>

int score_english(ba *text)
{
	int res = 0;
	for (int i = 0; i < text->len; i++)
		res += isalnum((text->val)[i]) || isspace((text->val)[i]);
	return res;
}

int main(int argc, char* argv[]) {
	if (argc != 2)
		return -1;

	ba *ct;
	ba *key;
	ct = ba_from_hex(argv[1]);
	uint8_t best_key;
	int max = 0;
	if (!ct)
		return -1;
	printf("decrypting...\n");
	for (uint16_t i = 0; i < 0xff; i++) {
		char buf[2*(ct->len)];
		snprintf(buf, 16, "%2x", i);
		key = ba_from_hex_n(buf, ct->len);
		ba_xor(key, ct);
		int sc = score_english(key);
		if (sc > max) {
			max = sc;
			best_key = i;
		}
	}

	char buf[2*(ct->len)];
	snprintf(buf, 16, "%2x", best_key);
	key = ba_from_hex_n(buf, ct->len);
	ba_xor(key, ct);
	ba_fprint_ascii(key, stdout, 0);
	printf("\n");

	ba_free(ct);
}
