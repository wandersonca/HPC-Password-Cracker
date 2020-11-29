#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>

char *hash(char *input, char outputBuffer[64])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    int i;

    SHA256(input, strlen(input), hash);
    for (i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}