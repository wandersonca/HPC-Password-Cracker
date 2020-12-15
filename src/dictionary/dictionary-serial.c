#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "dictionary-util.c"
#include "../hash/hash.h"
#include "../globals.h"

int compare_candidates(FILE **file, char *password_hash, int verbose);

/* 
    dictionary_crack()
        - Serial Implementation

        password_hash:      hashed password string to crack
        dictionary_path:    full path, including filename (serial)
        verbose:            set to 1 for verbose mode
*/
int dictionary_crack(char *password_hash, char *dictionary_path, int verbose)
{
    // Print input parameters 
    if( verbose )
    {
        printf("\n>>> Using dictionary path: %s\n", dictionary_path);
        print_password_hash(password_hash);
    }

    // Open file
    FILE *file = fopen(dictionary_path, "r");

    // do calculation
    int result = compare_candidates(&file, password_hash, verbose);

    if(result == NOT_FOUND)
        print_not_found(verbose);

    // cleanup
    fclose(file);
    return result;
}

/* 
    compare_candidates()
        - 1. manages iterating through the dictionary file and initiating the has comparisons
        - 2. returns the result value (FOUND or NOT_FOUND) and the plain text password, if found

        file:               pointer to the dictionary file in memory
        password_hash:      hashed value of the password to be cracked
        verbose:            set to 1 for verbose mode

*/
int compare_candidates(FILE **file, char *password_hash, int verbose)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, *file)) != -1)
    {
        char *candidate_buffer = NULL;
        remove_new_line(line, &candidate_buffer);
     
        int result = do_comparison(password_hash, candidate_buffer, verbose);

        if(result == FOUND)
        {
            return FOUND;
        }
            
    }
    return NOT_FOUND;
}
