#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "bruteforce-util.c"
#include "../hash/hash.h"
#include "../globals.h"

/**
* Brute force attack entry function 
*
* Enhanced OMP solution based on the bruteforce-mpi.c
*
* @param password_hash - hashed each character in password with sha256 values and hold them with this buffer.
* @param characters - patters to compare with which relies on the option we have picked in the main.c program.
* @param password_max_length - by default is 4, we might have -c N passes in as we are testing password with length N.
* @param verbose - options to print out debug info
* @return result - 1 indicates not found, 0 indicates found
*/
int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    // Input Calculations
    int number_of_characters = strlen(characters);
    if (verbose)  print_stats(password_hash, characters, number_of_characters, password_max_length);

    // Program counters and flags
    int result = NOT_FOUND;
    int i, j;

    for (i = 1; i <= password_max_length && result > 0; i++)
    {
        // Calculate the number of permutations we'll need to calculate
        long possibilities = calculate_possibilities(number_of_characters, i, verbose, 0);

        // split up for loop for chunking work
        for (j = 0; j < possibilities && result > 0;)
        {
            // Calculate and execute next chunk of work
            int nextStep = calculate_next_step(j, possibilities, CHUNK_SIZE);
            int k;
            #pragma omp parallel
            {
                #pragma omp for schedule(auto)
                for (k = j; k < nextStep; k++)
                {
                    // generate password, hash it, then compare it
                    unsigned char buffer[65];
                    char passwordToTest[i + 1];
                    generate_password(i, characters, number_of_characters, k, passwordToTest);
                    hash(passwordToTest, buffer);
                    if (!strcmp(password_hash, buffer))
                    {
                        #pragma omp critical
                        {
                            printf("Password found: %s\n", passwordToTest);
                            result = FOUND;
                        }
                    }
                }
            }
            j=nextStep;
        }
    }

    // Print not found result
    if (result == NOT_FOUND)
    {
         printf("Password not found.\n");
    }
    return result;
}