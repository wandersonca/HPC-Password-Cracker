#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../hash/hash.h"
#include "dictionary-util.c"
#include "mpi.h"

int dictionary_crack(char* password_hash, char *dictionary_path, int verbose)
{
  int         my_rank;       /* rank of process      */
  int         p;             /* number of processes  */
  int         source;        /* rank of sender       */
  int         dest;          /* rank of receiver     */
  int         tag = 0;       /* tag for messages     */
  char        message[100];  /* storage for message  */
  MPI_Status  status;        /* status for receive   */

  /* Start up MPI */
  MPI_Init(NULL, NULL);

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (my_rank != 0)
    {
      /* Create message */
      sprintf(message, "Greetings from process %d!", my_rank);
      dest = 0;
      /* Use strlen+1 so that '\0' gets transmitted */
      MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
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