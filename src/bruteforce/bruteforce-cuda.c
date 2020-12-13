#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce-util.c"

int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    static unsigned char buffer[65];
    int number_of_characters = strlen(characters);

    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    printf("Calculating to a length of %d\n", password_max_length);

    int i, j, k, result;
    result = 1;
    for (i = 1; i <= password_max_length; i++)
    {
        long possibilities = (long)pow(number_of_characters, i);
        if (verbose)
        {
            printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
        }
        char passwordToTest[i+1];
        for (j = 0; j < possibilities; j++)
        {
            strcpy(passwordToTest, "");
            int val = j;
            for (k = 0; k < i; k++)
            {
                passwordToTest[k] = characters[val % number_of_characters];
                val = val / number_of_characters;
            }
            passwordToTest[i]='\0';
            batch_hash(passwordToTest, buffer, i, 1);
            if (!strcmp(password_hash, buffer))
            {
                printf("Password found: %s\n", passwordToTest);
                result = 0;
                return result; // want to break out of these nested for loops...
            }
        }
    }
    if (result)
    {
      printf("Password not found.\n");
    }
    return result;
}