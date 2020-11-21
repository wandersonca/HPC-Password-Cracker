#include <string.h>
#include <openssl/sha.h>

char *hash(char *input, char outputBuffer[64])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256(input, strlen(input), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    //outputBuffer[64] = 0;
}