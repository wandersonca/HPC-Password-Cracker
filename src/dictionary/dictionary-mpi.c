#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../hash/hash.h"
#include "dictionary-util.c"
#include "mpi.h"

#define N 10
#define BUF_SIZE 80
#define FILE_PREFIX "file_0"

int dictionary_crack(char *password_hash, char *dictionary_path, int verbose)
{
  int my_rank;       /* rank of process      */
  int p;             /* number of processes  */
  int source;        /* rank of sender       */
  int dest;          /* rank of receiver     */
  int tag = 0;       /* tag for messages     */
  char message[100]; /* storage for message  */
  MPI_Status status; /* status for receive   */

  int i;

  /* Start up MPI */
  MPI_Init(NULL, NULL);

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  // if (my_rank != 0)
  // {
  //   /* Create message */
  //   sprintf(message, "Greetings from process %d!", my_rank);
  //   dest = 0;
  //   /* Use strlen+1 so that '\0' gets transmitted */
  //   MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  // }

  // else
  // { /* my_rank == 0 */
  //   for (source = 1; source < p; source++)
  //   {
  //     MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
  //     printf("%s\n", message);
  //   }
  // }

  /* CONFIRM the password_hash to all the processes */
  if (verbose)
  {
    printf("%d: I know the password_hash is: %s\n", my_rank, password_hash);
  }

  /* we know we have "p" processes */
  /* we know we have "N" password candidates */
  //int work_size = N / p;
  //int leftovers = N % p;

  static unsigned char candidate_hash[64];

  FILE *file;
  char *line = NULL;
  char *candidate_buffer = NULL;
  size_t len = 0;
  ssize_t read;

  char *dictionary_file = NULL;
  int filename_length = strlen(dictionary_path) + 1;

  dictionary_file = (char *)malloc(sizeof(char) * (filename_length));
  sprintf(dictionary_file, "%s/%s%d\0", dictionary_path, FILE_PREFIX, my_rank);

  file = fopen(dictionary_file, "r");
  if (file == NULL)
  {
    printf("Error reading dictionary file: %s\n", dictionary_file);
    printf("Exiting with error code %d.\n", EXIT_FAILURE);
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  printf("\n>>> Using dictionary path: %s\n\n", dictionary_file);

  if (verbose)
  {
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Looking for this password:\t\t\t\t\t%s\n", password_hash);
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
  }
  i = 0;

  while ((read = getline(&line, &len, file)) != -1)
  {

    remove_new_line(line, &candidate_buffer);
    hash(candidate_buffer, candidate_hash);

    if (verbose)
    {
      printf("%d: Password candidate from file:\t%16s\t--->\t%s\n", my_rank, candidate_buffer, candidate_hash);
    }

    if (!strcmp(password_hash, candidate_hash))
    {
      printf("\n%d: SUCCESS!!\tPassword found: %s\n", my_rank, candidate_buffer);

      free(candidate_buffer);
      fclose(file);
      return 0;
    }

    free(candidate_buffer);
  }

  fclose(file);

  printf("\n");

  /* Shut down MPI */
  MPI_Finalize();

  return 1;
}