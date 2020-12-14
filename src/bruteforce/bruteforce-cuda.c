#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce-util.c"

#define BATCH_SIZE 10000

/*
** @Author: William Anderson
**
** Brute force attack entry function 
**
** @CHUNK_SIZE is pre-defined, but could be adjusted
**
** @password_hash - hashed each character in password with sha256 values and hold them with this buffer.
** @characters - patters to compare with which relies on the option we have picked in the main.c program.
** @password_max_length - by default is 4, we might have -c N passes in as we are testing password with lengh N.
** @verbose - options to print out debug info
** @return result - 1 indicates not found, 0 indicates found
*/

int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    static unsigned char buffer[65];
    int number_of_characters = strlen(characters);

    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    printf("Calculating to a length of %d\n", password_max_length);

    int i, j, k, l, result;
    result = 1;
    for (i = 1; i <= password_max_length; i++)
    {
        long possibilities = (long)pow(number_of_characters, i);
        if (verbose)
        {
            printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
        }
        char passwordToTest[i+1];
        for (j = 0; j < possibilities;)
        {
            int nextStep = j + BATCH_SIZE;
            char inputArray[BATCH_SIZE * i];
            char outputArray[BATCH_SIZE * 65];
            for(k = 0; k < BATCH_SIZE && k+j < possibilities; k++) 
            {
                int val = j + k;
                for (l = 0; l < i; l++)
                {
                    inputArray[(i*k) + l] = characters[val % number_of_characters];
                    val = val / number_of_characters;
                }
            }

            //printf("%.*s\n", BATCH_SIZE * i, inputArray);
            batch_hash(inputArray, outputArray, i, BATCH_SIZE);
            
            for(k = 0; k < BATCH_SIZE && k+j < possibilities; k++) 
            {
                //printf("Password: %s\n", outputArray + k*65);
                if (!strcmp(password_hash, outputArray + k*65))
                {
                    printf("Password found: %s\n", outputArray + k*65);
                    result = 0;
                    return result; // want to break out of these nested for loops...
                }
            }
            j+=k;
        }
    }
    if (result)
    {
      printf("Password not found.\n");
    }
    return result;
}