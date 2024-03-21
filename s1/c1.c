#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int hex_to_base64(const char* hex, char** b64)
{
    const char b64table[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int length = strlen(hex);
    int block_count = length / 6;
    int res_length = block_count*4;
    int leftover_length = length % 6;
    if(leftover_length%2 == 1) {
        fprintf(stderr, "bad sequence length\n");
        return -1;
    }

    char * res = (char*)(malloc(sizeof(char)*res_length+2));

    for(int i = 0; i < block_count+(leftover_length != 0); i++) {
        int end_ix = i < block_count ? 6 : leftover_length;
        char block[6+1];
        memcpy(block, hex+(6*i), end_ix);
        block[end_ix] = '\0';
        
        int block_hex = strtol(block, NULL, 16);
        block_hex <<= 4*(6-end_ix);
        
        for(int j = 0; j < 4; j++) {
            char b64 = b64table[(block_hex >> 6*(3-j))& 0x3f];
            res[4*i+j] = j <= end_ix/2 ? b64 : '=';
        }
    }
    *b64 = res;
    return 0;
}

int main(int argc, char* argv[]) 
{
    char* b64;
    int ret = hex_to_base64(argv[1], &b64);
    if(ret == 0) {
        printf("%s\n", b64);
    }
    return 0;
}
