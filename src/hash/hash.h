#ifndef HASH_H
#define HASH_H

void hash(char *input, char output_buffer[64]);
void batch_hash(char *input, char *output, int password_length, int batch_size);

#endif //HASH_H