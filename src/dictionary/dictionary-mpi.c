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
  int my_rank; 
  //int p;    // don't seem to use this ... 

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  //MPI_Comm_size(MPI_COMM_WORLD, &p);    // don't seem to use this ...

  if (verbose)
    printf("\n>>> Using dictionary path: %s\n", dictionary_path);

  if (my_rank == 0 && verbose)
    print_password_hash(password_hash);

  if (verbose)
    MPI_Barrier(MPI_COMM_WORLD);

  static unsigned char candidate_hash[64];

  int result = NOT_FOUND;       /* default: match not found */
  int file_failure = SUCCESS;   /* default: no failure */

  char *password = NULL;

  FILE *file = NULL;
  char *dictionary_file_name = NULL;

  set_mpi_dictionary_filename(dictionary_path, my_rank, &dictionary_file_name);
  open_dictionary_file(dictionary_file_name, &file, MPI, &file_failure);

  if (file_failure)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  compare_candidates(&file, password_hash, MPI, verbose, &result, &password);

  close_dictionary_file(&file);
  free(dictionary_file_name);

  int final_result;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  /* only the root process should print the result */
  if(my_rank == 0)
  {
    if(result == NOT_FOUND)
        print_not_found(verbose);
    else if(result == FOUND)
        print_password_found(password, verbose);
  }

  MPI_Finalize();

  return final_result;
}