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
    outputBuffer[64] = '\0';
}

#ifdef BUILD_CUDA
#include "./cuda-hash/sha256.cuh"
#include "./cuda-hash/config.h"

char *batch_hash(char *input, char *outputBuffer, int passwordLength, int batchSize)
{
    unsigned char hashArray[SHA256_DIGEST_LENGTH * batchSize];
    mcm_cuda_sha256_hash_batch(input, passwordLength, hashArray, batchSize);
    int i, j;
    for(i=0; i < batchSize; i++) {
        for (j = 0; j < SHA256_DIGEST_LENGTH; ++j)
        {
            sprintf(outputBuffer + (i* 65) + (j * 2), "%02x", hashArray[(i*SHA256_DIGEST_LENGTH) + j]);
        }
        outputBuffer[((i + 1) * 65)-1] = '\0';
    }
}
#endif //BUILD_CUDA