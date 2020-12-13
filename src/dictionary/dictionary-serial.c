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
    int result = NOT_FOUND;         /* default: match not found */
    int file_failure = SUCCESS;     /* default: no failure */

    char *password = NULL;

    FILE *file = NULL;
    open_dictionary_file(dictionary_path, &file, SERIAL, &file_failure);

    if( verbose )
    {
        printf("\n>>> Using dictionary path: %s\n", dictionary_path);
        print_password_hash(password_hash);
    }

    compare_candidates(&file, password_hash, SERIAL, verbose, &result, &password);

    close_dictionary_file(&file);

    if(result == NOT_FOUND)
        print_not_found(verbose);
    else if(result == FOUND)
        print_password_found(password, verbose);

    free(password);

    return result;
}