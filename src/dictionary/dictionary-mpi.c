#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../hash/hash.h"
#include "dictionary-util.c"
#include "mpi.h"

#define N 10
#define BUF_SIZE 80
#define FILE_PREFIX "file_" 


int dictionary_crack(char *password_hash, char *dictionary_path, int verbose);

void set_mpi_dictionary_filename(char *dictionary_path, int rank, char **filename);
int mpi_check_if_found(int result);
void compare_candidates(FILE **file, char *password_hash, int verbose, int *result, char **password_text);



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

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

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

  compare_candidates(&file, password_hash, verbose, &result, &password);

  close_dictionary_file(&file);
  free(dictionary_file_name);

  int final_result;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&result, &final_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  /* this needs to be local result, so that the process with the value can display it */
  if(result == FOUND)
    print_password_found(password, verbose);

  if(final_result == NOT_FOUND)
    print_not_found(verbose);

  MPI_Finalize();

  free(password);

  return final_result;
}

/* 
    mpi_result_check()
        - a status check for the entire communicator
        --- checks if another process has found the password match
        --- allows the process that found the match to report its success

        my_result:    the status of the process that is reporting in (FOUND or NOT_FOUND)
*/
int mpi_result_check(int my_result)
{
  int result_check;
  MPI_Allreduce(&my_result, &result_check, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  return result_check;
}


/* 
    set_mpi_dictionary_filename()
        - sets the name of the MPI dictionary filename for a given process

        dictionary_path:    the directory where the split files are located
        rank:               the process rank (needed for the file selection)
        filename:           the filename for this process to work, including the full path
*/
void set_mpi_dictionary_filename(char *dictionary_path, int rank, char **filename)
{
    int filename_length = strlen(dictionary_path) + 1;

    *filename = (char *)malloc(sizeof(char) * (filename_length));
    if(rank<10) 
    {
      sprintf(*filename, "%s/%s0%d\0", dictionary_path, FILE_PREFIX, rank);
    }
    else 
    {
      sprintf(*filename, "%s/%s%d\0", dictionary_path, FILE_PREFIX, rank);
    }
}




/* 
    compare_candidates()
        - 1. manages iterating through the dictionary file and initiating the has comparisons
        - 2. returns the result value (FOUND or NOT_FOUND) and the plain text password, if found

        file:               pointer to the dictionary file in memory
        password_hash:      hashed value of the password to be cracked
        verbose:            set to 1 for verbose mode
        result:             (output) FOUND or NOT_FOUND result
        password_text:      (output) plain text of the discovered password

*/
void compare_candidates(FILE **file, char *password_hash, int verbose, int *result, char **password_text)
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  int count = 0;  /* for implementations that require a counter */

  while ((read = getline(&line, &len, *file)) != -1)
  {
    char *candidate_buffer = NULL;
    remove_new_line(line, &candidate_buffer);

    /* First check if it is already FOUND, and return if FOUND */
    if(count == MPI_COUNT_LIMIT)
    {
        if( mpi_result_check(NOT_FOUND) == FOUND)
            return;

        count = 0;
    }

    /* if NOT_FOUND, keep looking */
    do_comparison(password_hash, candidate_buffer, verbose, result, password_text);
    count++;

    /* This STOPS the processing of the file on the process that FOUND the password */
    if(*result == FOUND)
    {
        /* report back that the match is found */
        mpi_result_check(FOUND);
        return;
    }      
  }
}

