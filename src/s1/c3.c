#include <ba.h>
#include <analysis.h>

int main(int argc, char* argv[])
{
	ba *ct;

	if (argc != 2)
		return -1;

	ct = ba_from_hex(argv[1]);

	if (!ct)
		return -1;

	ct = decrypt_scxor(ct);
	ba_fprint_ascii(ct, stdout, 0);
	printf("\n");
	ba_free(ct);
}
