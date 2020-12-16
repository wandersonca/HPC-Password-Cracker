#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "../globals.h"
#include "bruteforce-util.c"

/**
* Brute force attack entry function 
*
* Enhanced two For-Loop implementation is used here.  All the possibilities is generated and comparision is happening along with the process.
*
* @param password_hash - hashed each character in password with sha256 values and hold them with this buffer.
* @param characters - patters to compare with which relies on the option we have picked in the main.c program.
* @param password_max_length - by default is 4, we might have -c N passes in as we are testing password with lengh N.
* @param verbose - options to print out debug info
* @return result - 1 indicates not found, 0 indicates found
*/
int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    // Input Calculations
    static unsigned char buffer[65];
    int number_of_characters = strlen(characters);
    if (verbose)
        print_stats(password_hash, characters, number_of_characters, password_max_length);

    // Program counters and flags
    int i, j, k, result;
    result = NOT_FOUND;

    for (i = 1; i <= password_max_length; i++)
    {
        // Calculate the number of permutations we'll need to calculate
        long possibilities = calculate_possibilities(number_of_characters, i, verbose, 0);
        char passwordToTest[i + 1];
        for (j = 0; j < possibilities; j++)
        {
            // generate password, hash it, then compare it
            generate_password(i, characters, number_of_characters, j, passwordToTest);
            hash(passwordToTest, buffer);
            if (!strcmp(password_hash, buffer))
            {
                printf("Password found: %s\n", passwordToTest);
                result = FOUND;
                // want to break out of these nested for loops
                return result; 
            }
        }
    }

    // Print not found result
    if (result == NOT_FOUND)
    {
        printf("Password not found.\n");
    }
    return result;
}