#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#define PASS_FOUND 0
#define PASS_NOT_FOUND 1

int bruteforce_crack(char* password_hash, char * characters, int password_max_length, int verbose);

#endif //BRUTEFORCE_H