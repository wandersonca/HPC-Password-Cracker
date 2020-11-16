#include <stdlib.h>
#include <string.h>
#include "hash.c"

int dictionary_crack(char* password_hash, char *dictionary_path, int verbose)
{
    printf("Using dictionary path %s.\n", dictionary_path);
    static unsigned char buffer[64];
    if(verbose) 
    {
            printf("Hash to compare: %s\n", password_hash);
    }

    for(int i=0; i < 1; i++)
    {
        // should read in these from a file...
        char passwordToTest[] = "test";
        hash(passwordToTest, buffer);
        if(verbose) 
        {
            printf("`%s` -> %s\n", passwordToTest, buffer);
        }

        if (!strcmp(password_hash, buffer))
        {
            printf("Password found: %s\n", passwordToTest);
            return 0;
        }        
    }
    return 1;
}