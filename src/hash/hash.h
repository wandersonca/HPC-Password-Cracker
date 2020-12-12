#ifndef HASH_H
#define HASH_H

char *hash(char *input, char outputBuffer[64]);
char *batch_hash(char *input, char *output, int passwordLength, int batchSize);

#endif //HASH_H