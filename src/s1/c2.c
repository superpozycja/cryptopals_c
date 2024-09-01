#include <ba.h>

int main(int argc, char* argv[])
{
	ba* a;
	ba* b;

	if (argc != 3)
		return -1;

	a = ba_from_hex(argv[1]);
	b = ba_from_hex(argv[2]);

	if (a == NULL || b == NULL) {
		return -1;
	}

	ba_xor(a, b);
	ba_fprint(a, stdout, 0);
	printf("\n");
	ba_free(a);
	ba_free(b);
	return 0;
}
