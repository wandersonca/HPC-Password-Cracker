#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
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

  int i, j, k, result;
  result = 1;
  for (i = 1; i <= password_max_length; i++)
  {
      long possibilities = (long)pow(number_of_characters, i);
      if (verbose && my_rank == 0)
      {
          printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
      }
      char passwordToTest[i+1];
      for (j = my_rank; j < possibilities; j+=p)
      {
          strcpy(passwordToTest, "");
          int val = j;
          for (k = 0; k < i; k++)
          {
              passwordToTest[k] = characters[val % number_of_characters];
              val = val / number_of_characters;
          }
          passwordToTest[i]='\0';
          hash(passwordToTest, buffer);
          if (!strcmp(password_hash, buffer))
          {
              printf("Password found: %s\n", passwordToTest);
              result = 0;
          }
      }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  int final_result;
  //MPI_Reduce(&result, &final_result, 1, MPI_Int, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  // Shutdown MPI
  MPI_Finalize();
  if (final_result && my_rank == 0)
  {
      printf("Password not found.\n");
  }
  return final_result;
}
