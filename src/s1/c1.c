#include <stdio.h>
#include <util.h>

int main(int argc, char* argv[])
{
	char* b64;
	int ret;

	if (hex_to_base64(&b64, argv[1]) == 0)
		printf("%s\n", b64);

	return 0;
}
