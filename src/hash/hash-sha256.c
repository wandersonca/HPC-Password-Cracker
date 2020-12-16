#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>

/**
 * Performs a hash using the SHA256 openssl library.
 *
 * @param input - the raw password to hash
 * @return the hashed password in the output_buffer.
 */
void hash(char *input, char output_buffer[64])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    int i;

    SHA256(input, strlen(input), hash);
    for (i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        sprintf(output_buffer + (i * 2), "%02x", hash[i]);
    }
    output_buffer[64] = '\0';
}

#ifdef BUILD_CUDA
#include "./cuda-hash/sha256.cuh"
#include "./cuda-hash/config.h"

/**
 * Hashes a batch of input using a CUDA library. 
 * 
 * @param input - an array of raw raw password to hash
 * @param output_buffer - the array of hashed passwords
 * @param password_length - how long the input passwords are, they must match in size
 * @param batch_size - how many passwords are in the array
 * @return the ... in the output_buffer.
 */
void batch_hash(char *input, char *output_buffer, int password_length, int batch_size)
{
    unsigned char hash_array[SHA256_DIGEST_LENGTH * batch_size];
    mcm_cuda_sha256_hash_batch(input, password_length, hash_array, batch_size);
    int i, j;
    for(i=0; i < batch_size; i++) {
        for (j = 0; j < SHA256_DIGEST_LENGTH; ++j)
        {
            sprintf(output_buffer + (i* 65) + (j * 2), "%02x", hash_array[(i*SHA256_DIGEST_LENGTH) + j]);
        }
        output_buffer[((i + 1) * 65)-1] = '\0';
    }
}
#endif //BUILD_CUDA