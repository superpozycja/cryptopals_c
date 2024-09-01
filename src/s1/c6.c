#include <stdio.h>
#include <ctype.h>
#include <ba.h>
#include <cryptanalysis.h>
#include <util.h>

float i_c(ba *ct)
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

float get_i_c_m(ba *ct, unsigned int m)
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
		res += i_c(chunk);
	}
	return res / m;
}

unsigned int guess_m(ba *ct)
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

void break_m(ba *ct, unsigned int m)
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
	
	int m = guess_m(ct);

	break_m(ct, m);

	ba_fprint_ascii(ct, stdout, 0);
	printf("\n");

	ba_free(ct);
	fclose(f);
	return 0;
}
