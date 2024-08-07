#include <stdio.h>
#include <ctype.h>
#include <ba.h>
#include <cryptanalysis.h>

#define MAXLEN 1000

void preprocess_string(char (*l)[MAXLEN])
{
	while (isspace(**l))
		*l++;
	for (int i = 0; i < strlen(*l); i++) {
		if ((*l)[i] == '\n') {
			(*l)[i] = '\0';
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		return -1;
	FILE *p = fopen(argv[1], "r");
	char l[MAXLEN];
	//char best[MAXLEN];
	unsigned long maxscore = 0;
	ba *ct;
	ba *best = (ba *) malloc(sizeof(ba));
	while (fgets(l, sizeof(l), p)) {
		preprocess_string(&l);
		ct = ba_from_hex(l);
		ct = decrypt_scxor(ct);
		unsigned long score = score_english(ct);
		if (score > maxscore) {
			maxscore = score;
			ba_copy(best, ct);
			//strcpy(best, l);
		}
	}
	ba_fprint_ascii(best, stdout, 0);
	printf("\n");

	ba_free(ct);
}
