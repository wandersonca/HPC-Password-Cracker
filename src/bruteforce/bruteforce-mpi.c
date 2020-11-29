/*
** @Author: Ma Luo
** @Date: Nov 29,2020
** @MPI version: 
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce-util.c"
#include <mpi.h>

int bruteforce_crack(char *password_hash, char *characters, int password_max_length, int verbose)
{
  int my_rank;       /* rank of process      */
  int p;             /* number of processes  */
  int source;        /* rank of sender       */
  int dest;          /* rank of receiver     */
  int tag = 0;       /* tag for messages     */
  char message[100]; /* storage for message  */
  MPI_Status status; /* status for receive   */
  long possibilities_local = 0;
  int number_of_characters = 0;

  /*Before MPI Starts*/
  printf("Brute force of hash: %s\n", password_hash);
  number_of_characters = strlen(characters);
  printf("Using %d characters: %s\n", number_of_characters, characters);
  static unsigned char buffer[65];
  printf("Calculating to a length of %d\n", password_max_length);

  /* Start up MPI */
  MPI_Init(NULL, NULL);

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  int i, j, k; //Individually working on each process here
  for (i = 1; i <= password_max_length; i++)
  {
    long possibilities = (long)pow(number_of_characters, i);
    if (verbose)
    {
      printf("Now calculating password length of %d, it has %ld possibilities\n", i, possibilities);
    }

    /*Split up the chunk by inexing*/
    int stepSize = (int) (possibilities / my_rank);
    int startIndex = my_rank * stepSize;
    int endIndex = 0;
    if (my_rank = p - 1)
    {
      endIndex = possibilities;
    }
    else
    {
      endIndex = startIndex + stepSize;
    }

    char passwordToTest[i];
    for (j = startIndex; j < endIndex; j++) //Split to n/p processes here
    {
      strcpy(passwordToTest, "");
      int val = j;
      for (k = 0; k < i; k++)
      {
        passwordToTest[k] = characters[val % number_of_characters];
        val = val / number_of_characters;
      }
      hash(passwordToTest, buffer);
      if (!strcmp(password_hash, buffer))
      {
        printf("Password found: %s\n", passwordToTest);
        return 0;
      }
    }
  }

  if (my_rank != 0)
  {
    sprintf(message, "Greetings from process %d!", my_rank);
    dest = 0;
    /* Use strlen+1 so that '\0' gets transmitted */
    MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  }

  else
  { /* my_rank == 0 */
    for (source = 1; source < p; source++)
    {
      MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
      printf("%s\n", message);
    }
  }

  /* Shut down MPI */
  MPI_Finalize();

  return 0;
}