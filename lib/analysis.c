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

float index_of_coincidence(ba *ct)
{
	unsigned int occur[256];
	unsigned int i;
	unsigned int t;
	float res;

	res = 0;
	memset(occur, 0, sizeof(occur));

	for (i = 0; i < ct->len; i++)
		occur[ct->val[i]]++;

	t = 0;
	for (i = 0; i < 256; i++)
		t += occur[i] * (occur[i] - 1);

	res = t / (float) (ct->len * (ct->len - 1));
	return res;
}

static float get_i_c_m(ba *ct, unsigned int m)
{
	float res;
	int i;

	res = 0;
	for (i = 0; i < m; i++) {
		ba *chunk = (ba *) malloc(sizeof(ba));
		int j;
		int k;

		chunk->len = ct->len / m;
		chunk->val = (uint8_t *) malloc(sizeof(uint8_t) * chunk->len);

		for (j = 0, k = 0; j < chunk->len &&  k + i < ct->len; j++, k += m)
			chunk->val[j] = ct->val[k + i];
		res += index_of_coincidence(chunk);
	}
	return res / m;
}

unsigned int guess_vigenere_keylen(ba *ct)
{
	unsigned int best;
	float max;
	int m;

	max = 0;
	for (m = 1; m < 40; m++) {
		float avg;

		avg = get_i_c_m(ct, m);
		if (avg > max) {
			max = avg;
			best = m;
		}
	}
	return best;
}
