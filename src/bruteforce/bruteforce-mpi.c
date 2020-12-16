#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "bruteforce-util.c"
#include "../hash/hash.h"
#include "../globals.h"

/**
* Brute force attack entry function 
*
* Enhanced MPI solution based on the bruteforce-serial.c
*
* @param password_hash - hashed each character in password with sha256 values and hold them with this buffer.
* @param characters - patters to compare with which relies on the option we have picked in the main.c program.
* @param password_max_length - by default is 4, we might have -c N passes in as we are testing password with length N.
* @param verbose - options to print out debug info
* @return result - 1 indicates not found, 0 indicates found
*/
int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
    // MPI Setup
    int my_rank;
    int p;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    // Input Calculations
    static unsigned char buffer[65];
    int number_of_characters = strlen(characters);
    if (verbose && my_rank == 0)  print_stats(password_hash, characters, number_of_characters, password_max_length);

    // Program counters and flags
    int i, j, k, result, collective_result;
    result = NOT_FOUND;
    collective_result = NOT_FOUND;

    for (i = 1; i <= password_max_length && collective_result > 0; i++)
    {
        // Calculate the number of permutations we'll need to calculate
        long possibilities = calculate_possibilities(number_of_characters, i, verbose, my_rank);
        char passwordToTest[i + 1];

        // split up for loop for chunking work
        for (j = my_rank; j < possibilities;)
        {
            // Only want to run an Allrecude if all processes are participating 
            if(possibilities > p)
            {  
                // Periodically check result to break early
                MPI_Allreduce(&result, &collective_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
                if (collective_result == 0)
                {
                    break;
                }
            }

            // Calculate and execute next chunk of work
            int nextStep = calculate_next_step(j, possibilities, CHUNK_SIZE);
            for (;j < nextStep; j+=p)
            {      
                // generate password, hash it, then compare it     
                generate_password(i, characters, number_of_characters, j, passwordToTest);
                hash(passwordToTest, buffer);
                if (!strcmp(password_hash, buffer))
                {
                    printf("Password found: %s\n", passwordToTest);
                    result = FOUND;
                }
            }
        }
    }

    // Print not found result
    MPI_Allreduce(&result, &collective_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    if (collective_result == NOT_FOUND && my_rank == 0)
    {
        printf("Password not found.\n");
    }
    
    // Shutdown MPI
    MPI_Finalize();
    return FOUND;
}