#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce.h"
#include "bruteforce-util.c"
#include <mpi.h>

int bruteforce_crack_sub(char *password_hash, char *characters, int password_max_length, int verbose, int my_rank, int p);

int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    // MPI Setup
    int my_rank;
    int p;
    int source;
    int dest;
    MPI_Status status;
    int number_of_characters = 0;
    int result = 0, final_result = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    static unsigned char buffer[65];
    number_of_characters = strlen(characters);

    if (my_rank == 0)
    {
        /*Before MPI Starts*/
        printf("Brute force of hash: %s\n", password_hash);
        printf("Using %d characters: %s\n", number_of_characters, characters);
        printf("Calculating to a length of %d\n", password_max_length);
    }

    result = bruteforce_crack_sub(password_hash, characters, password_max_length, verbose, my_rank, p);
    
    MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    // Shutdown MPI
    MPI_Finalize();
    // if (final_result == 0)
    // {
    //     printf("Password not found overall.\n");
    // }
    return 0;
}


int bruteforce_crack_sub(char *password_hash, char *characters, int password_max_length, int verbose, int my_rank, int p)
{
    static unsigned char buffer[65];
    int number_of_characters = strlen(characters);
    int i, j, k, result, found, final_result;
    result = 0; //By defualt it is one
    found = 0;

    for (i = 1; i <= password_max_length; i++)
    {
        long possibilities = (long)pow(number_of_characters, i);
        if (verbose && my_rank == 0)
        {
            printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
        }
        char passwordToTest[i + 1];
        for (j = my_rank; j < possibilities;)
        {
            //MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
            if (mpi_result_check(result) > 0)
            {
                printf("Found pass already...\n");
                return 0; //Skip the next processes if we have found the result
            }
            int nextStep = j + 100000;
            for (; j < nextStep; j += p)
            {
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
    }

    if (found == 0)
    {
        printf("Password not found at rank %d\n", my_rank);
    }

    return result;
}