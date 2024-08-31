#include "cryptanalysis.h"

unsigned long score_letter(char c)
{
	/* this is approximate, however this shouldnt really make a difference
	 * unless you have multiple ciphertexts very close to each other as 
	 * far as the score is concerned - meaning this is enough to weed out
	 * junk (nonenglish) byte sequences */
	unsigned long freqs[] = {
		82, 15, 28, 43, 130,
		22, 20, 61, 70, 1,
		8, 40, 24, 67, 75,
		19, 1, 60, 63, 91,
		28, 10, 24, 2, 20, 
		1};

	c = tolower(c);
	if (c == ' ')
		return 200;

	if (isalpha(c))
		return freqs[c-'a'];

	return 0;
}

unsigned long score_english(ba *text)
{
	unsigned long res;
	int i;

	res = 0;
	for (i = 0; i < text->len; i++)
		res += score_letter((text->val)[i]);
	return res;
}

ba *decrypt_scxor(ba *ct)
{
	char buf[2*(ct->len)];
	uint8_t best_key;
	uint16_t i;
	ba *key;
	int max;

	max = 0;
	for (i = 0; i < 0xff; i++) {
		char buf[2*(ct->len)];
		snprintf(buf, 16, "%2x", i);
		key = ba_from_hex_n(buf, ct->len);
		ba_xor(key, ct);
		unsigned long sc = score_english(key);
		if (sc > max) {
			max = sc;
			best_key = i;
		}
	}

	snprintf(buf, 16, "%2x", best_key);
	key = ba_from_hex_n(buf, ct->len);
	ba_xor(ct, key);
	ba_free(key);
	return ct;
}

void encrypt_rkxor(ba *pt, ba *key)
{
	ba *key_exp;
	int i;

	key_exp->len = pt->len;
	key_exp->val = (uint8_t *) malloc(sizeof(uint8_t) * key_exp->len);
	for (i = 0; i < key_exp->len; i++)
		key_exp->val[i] = key->val[i % (key->len)];
	ba_xor(pt, key_exp);
}
