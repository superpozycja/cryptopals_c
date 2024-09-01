#include <stdio.h>
#include <ctype.h>
#include <ba.h>
#include <analysis.h>

#define MAXLEN 1000

void preprocess_string(char (*l)[MAXLEN])
{
	int i;

	while (isspace(**l))
		*l++;
	for (i = 0; i < strlen(*l); i++) {
		if ((*l)[i] == '\n') {
			(*l)[i] = '\0';
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	unsigned long maxscore;
	char l[MAXLEN];
	ba *best;
	FILE *p;
	ba *ct;

	if (argc != 2)
		return -1;

	p = fopen(argv[1], "r");
	maxscore = 0;
	best = (ba *) malloc(sizeof(ba));

	while (fgets(l, sizeof(l), p)) {
		unsigned long score;

		preprocess_string(&l);
		ct = ba_from_hex(l);
		ct = decrypt_scxor(ct);
		score = score_english(ct);
		if (score > maxscore) {
			maxscore = score;
			ba_copy(best, ct);
		}
	}
	ba_fprint_ascii(best, stdout, 0);
	printf("\n");

	ba_free(ct);
}
