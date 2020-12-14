#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "../globals.h"
#include "bruteforce-util.c"

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
        long possibilities = calculate_possibilities(number_of_characters, i, verbose);
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
                return result; // want to break out of these nested for loops...
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