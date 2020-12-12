#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../hash/hash.h"
#include "dictionary-util.c"


int dictionary_crack(char* password_hash, char *dictionary_path, int verbose)
{
    int result = 1; /* not found */

    FILE *file = NULL;
    open_dictionary_file(dictionary_path, &file);
 
    printf("\n>>> Using dictionary path: %s\n", dictionary_path);

    if(verbose)
        print_password_hash(password_hash);

    compare_candidates(&file, password_hash, verbose, &result);

    close_dictionary_file(&file);

    if(result)
        printf("\n>>> Password not found. <<<\n");

    printf("\n");

    return result;
}