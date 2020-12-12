#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce.h"
#include "bruteforce-util.c"
#include <mpi.h>

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
    
    if (final_result == 0)
    {
        result = bruteforce_crack_sub(password_hash, characters, password_max_length, verbose, my_rank, p);
        MPI_Barrier(MPI_COMM_WORLD); //This would wait for all the results then do an all reduce.
        MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    }
    // Shutdown MPI
    MPI_Finalize();
    if (final_result && my_rank == 0)
    {
        printf("Password not found overall.\n");
    }
    //if(final_result > 0) {return 2;}
    // bruteforce_crack_main(password_hash, characters, password_max_length, verbose, my_rank, p);
}
