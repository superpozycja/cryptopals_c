#include <stdio.h>
#include <ctype.h>
#include <ba.h>
#include <analysis.h>

int main()
{
	ba *a, *b;

	a = ba_from_string("Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal");
	b = ba_from_string("ICE");
	encrypt_rkxor(a, b);
	ba_fprint(a, stdout, 0);
	printf("\n");
}
