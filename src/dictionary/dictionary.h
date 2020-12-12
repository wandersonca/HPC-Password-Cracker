#ifndef DICTIONARY_H
#define DICTIONARY_H

/* "mode" definitions */
#define SERIAL  0
#define MPI     1
#define OMP     2
#define CUDA    3
#define HYBRID  4

int dictionary_crack(char *password_hash, char *dictionary_path, int verbose);

int remove_new_line(char *input, char **output);
void print_password_hash(char *password_hash);

void open_dictionary_file(char *dictionary_path, FILE **file, int mode, int *failure);
void compare_candidates(FILE **file, char *password_hash, int verbose, int *result);
void close_dictionary_file(FILE **file);

#endif //DICTIONARY_H