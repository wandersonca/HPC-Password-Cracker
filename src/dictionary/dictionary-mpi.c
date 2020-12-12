#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../hash/hash.h"
#include "dictionary-util.c"
#include "mpi.h"

#define N 10
#define BUF_SIZE 80
//#define FILE_PREFIX "file_0"

/* 
    dictionary_crack()
        - MPI Implementation

        password_hash:      hashed password string to crack
        dictionary_path:    path to the directory with the "p" split files
        verbose:            set to 1 for verbose mode
*/
int dictionary_crack(char *password_hash, char *dictionary_path, int verbose)
{
  int my_rank;       /* rank of process      */
  int p;             /* number of processes  */
  int source;        /* rank of sender       */
  int dest;          /* rank of receiver     */
  int tag = 0;       /* tag for messages     */
  char message[100]; /* storage for message  */
  MPI_Status status; /* status for receive   */

  /* Start up MPI */
  MPI_Init(NULL, NULL);

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (verbose)
    printf("\n>>> Using dictionary path: %s\n", dictionary_path);

  if (my_rank == 0)
    print_password_hash(password_hash);

  if (verbose)
    MPI_Barrier(MPI_COMM_WORLD);

  static unsigned char candidate_hash[64];

  int result = 1; /* match not found */
  int file_failure = 0;

  FILE *file = NULL;
  char *dictionary_file_name = NULL;

  set_mpi_dictionary_filename(dictionary_path, my_rank, &dictionary_file_name);

  open_dictionary_file(dictionary_file_name, &file, MPI, &file_failure);

  if (file_failure)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  compare_candidates(&file, password_hash, verbose, &result);

  close_dictionary_file(&file);

  int final_result;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  // Shutdown MPI
  MPI_Finalize();

  if (final_result && my_rank == 0)
    print_not_found();

  return final_result;
}