#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce-util.c"
#include <mpi.h>

#define CHUNK_SIZE 100000

/*
* @Author: William Anderson
*
* Brute force attack entry function 
*
* Enhanced MPI solution based on the bruteforce-serial.c
*
* @CHUNK_SIZE is pre-defined, but could be adjusted
*
* @password_hash - hashed each character in password with sha256 values and hold them with this buffer.
* @characters - patters to compare with which relies on the option we have picked in the main.c program.
* @password_max_length - by default is 4, we might have -c N passes in as we are testing password with lengh N.
* @verbose - options to print out debug info
* @return result - 1 indicates not found, 0 indicates found
*/

int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    // MPI Setup
    int my_rank;
    int p;
    int source;
    int dest;
    MPI_Status status;
    int number_of_characters = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    static unsigned char buffer[65];
    number_of_characters = strlen(characters);

    if (my_rank == 0)
    {
        printf("Brute force of hash: %s\n", password_hash);
        printf("Using %d characters: %s\n", number_of_characters, characters);
        printf("Calculating to a length of %d\n", password_max_length);
    }

    int i, j, k, result, final_result;
    result = 1;
    final_result = 1;
    for (i = 1; i <= password_max_length; i++)
    {
        if (final_result == 0)
        {
            break;
        }
        long possibilities = (long)pow(number_of_characters, i);
        if (verbose && my_rank == 0)
        {
            printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
        }
        char passwordToTest[i + 1];
        for (j = my_rank; j < possibilities;)
        {
            MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
            if (final_result == 0)
            {
                break;
            }
            int nextStep = climbToMax(j, possibilities, CHUNK_SIZE);
            for (; j < nextStep; j += p)
            {
                int val = j;
                for (k = 0; k < i; k++)
                {
                    val = assignCharInBuffer(passwordToTest, characters, k, number_of_characters, val);
                }
                passwordToTest[i] = '\0';
                hash(passwordToTest, buffer);
                if (!strcmp(password_hash, buffer))
                {
                    printf("Password found: %s\n", passwordToTest);
                    result = 0;
                }
                /*
                * We ahve tried to use the common methods here, but mpi does not support a return statemnt in the middle of logic
                * An error will be thrown, [Exit code: 1] 
                */
                //result = findPasswordOrNo(password_hash, buffer, passwordToTest);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    // Shutdown MPI
    MPI_Finalize();
    if (final_result && my_rank == 0)
    {
        printf("Password not found.\n");
    }
    return final_result;
}