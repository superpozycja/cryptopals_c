#include "block.h"


static int split_to_chunks(ba *src, uint16_t block_size, uint16_t chunks_n,
                           ba *dest[chunks_n])
{
	int i;

	for (i = 0; i < chunks_n; i++)
		dest[i] = ba_alloc(block_size);

	i = 0;
	while (i < src->len)
		dest[(i)/block_size]->val[(i++)%block_size] = src->val[i];

	while (i < block_size * chunks_n)
		dest[i/chunks_n]->val[(i++)%chunks_n] = 0;

	return 0;
}

static int unsplit_from_chunks(ba **dest, uint16_t block_size, uint16_t chunks_n,
                               ba *src[chunks_n])
{
	int ct_size;
	int i;

	ct_size = block_size * chunks_n;
	*dest = ba_alloc(ct_size);

	i = 0;
	while (i < ct_size)
		(*dest)->val[i++] = src[(i)/block_size]->val[(i)%block_size];

	return 0;
}

int encrypt_ecb(ba *plaintext, ba *key, ba **ciphertext, uint16_t block_size,
                int (* encrypt) (ba *, ba *, ba **))
{
	uint16_t chunks_n = (plaintext->len / block_size) +
	                    (plaintext->len % block_size == 1);
	ba *ct_chunks[chunks_n];
	ba *chunks[chunks_n];

	split_to_chunks(plaintext, block_size, chunks_n, chunks);
	for (int i = 0; i < chunks_n; i++) {
		ct_chunks[i] = ba_alloc(block_size);

		encrypt(chunks[i], key, &ct_chunks[i]);
		ba_free(chunks[i]);
	}

	unsplit_from_chunks(ciphertext, block_size, chunks_n, ct_chunks);

	for (int i = 0; i < chunks_n; i++)
		ba_free(ct_chunks[i]);

	return 0;
}

int decrypt_ecb(ba *ciphertext, ba *key, ba **plaintext, uint16_t block_size,
                int (* decrypt) (ba *, ba *, ba **))
{
	/* this is symmetric */
	return encrypt_ecb(ciphertext, key, plaintext, block_size, decrypt);
}
