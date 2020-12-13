#ifndef DICTIONARY_H
#define DICTIONARY_H

/* general definitions */
#define FOUND       0
#define NOT_FOUND   1

#define SUCCESS     0
#define FAILURE     1

/* "mode" definitions */
#define SERIAL  0
#define MPI     1
#define OMP     2
#define CUDA    3
#define HYBRID  4

int dictionary_crack(char *password_hash, char *dictionary_path, int verbose);

void remove_new_line(char *input, char **output);
void return_password_text(char *input, char **output);

void print_password_hash(char *password_hash);
void print_password_found(char *match, int verbose);
void print_not_found(int verbose);

void open_dictionary_file(char *dictionary_path, FILE **file, int mode, int *failure);
void compare_candidates(FILE **file, char *password_hash, int mode, int verbose, int *result, char **password_text);
void do_comparison(char *password_hash, char *candidate_buffer, int verbose, int *result, char **password_text);
void close_dictionary_file(FILE **file);


#endif //DICTIONARY_H