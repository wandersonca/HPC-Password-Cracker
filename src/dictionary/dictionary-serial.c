#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../hash/hash.h"
#include "dictionary-util.c"

/* 
    dictionary_crack()
        - Serial Implementation

        password_hash:      hashed password string to crack
        dictionary_path:    full path, including filename (serial)
        verbose:            set to 1 for verbose mode
*/
int dictionary_crack(char *password_hash, char *dictionary_path, int verbose)
{
    int result = 1; /* match not found */
    int failure = 0;

    FILE *file = NULL;
    open_dictionary_file(dictionary_path, &file, SERIAL, &failure);

    if( verbose )
        printf("\n>>> Using dictionary path: %s\n", dictionary_path);

    print_password_hash(password_hash);

    compare_candidates(&file, password_hash, verbose, &result);

    close_dictionary_file(&file);

    if (result)
        print_not_found();

    printf("\n");

    return result;
}