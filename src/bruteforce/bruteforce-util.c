#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include <mpi.h>

// Add any common functionality here...

int bruteforce_crack_sub(char *password_hash, char *characters, int password_max_length, int verbose, int my_rank, int p){
    static unsigned char buffer[65];
    int number_of_characters = strlen(characters);
    int i, j, k, result, found, final_result;
    result = 0;//By defualt it is one
    found = 0;

    for (i = 1; i <= password_max_length; i++)
    {
        long possibilities = (long)pow(number_of_characters, i);
        if (verbose && my_rank == 0)
        {
            printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
        }
        char passwordToTest[i + 1];
        for (j = my_rank; j < possibilities; j += p)
        {

            //Give a counter check here and call MPI_Allreduce if meet
            //Give a stop flag and continue the next part only if we have results checked.

            strcpy(passwordToTest, "");
            int val = j;
            for (k = 0; k < i; k++)
            {
                passwordToTest[k] = characters[val % number_of_characters];
                val = val / number_of_characters;
            }
            passwordToTest[i] = '\0';
            hash(passwordToTest, buffer);
            if (!strcmp(password_hash, buffer))
            {
                printf("Password found: %s at rank %d\n", passwordToTest, my_rank);
                result = 1; //If found returns 0
                found = 1;
            }
        }
    }

    if (found == 0)
    {
        printf("Password not found at rank %d\n", my_rank);
    }

    return result;
}