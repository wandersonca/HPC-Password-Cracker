#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <mpi.h>
#include "dictionary-util.c"
#include "../hash/hash.h"
#include "../globals.h"

char *set_mpi_dictionary_filename(char *dictionary_path, int rank);
int mpi_check_if_found(int result);
int compare_candidates(FILE **file, char *password_hash, int verbose, int p);

/**
 * dictionary_crack() - MPI Implementation
 *
 * The MPI implementation of the dictionary attack.
 *
 * @param password_hash is the hashed password string to crack.
 * @param dictionary_path is the path to the directory with the "p" split files.
 * @param verbose is a flag for verbose mode. Set to 1 to enable.
 * @return the result as an integer value, FOUND (0) or NOT_FOUND (1).
 */
int dictionary_crack(char *password_hash, char *dictionary_path, int verbose)
{
  // MPI Setup
  int my_rank;
  int p;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  // Print input parameters
  if (verbose && my_rank == 0)
  {
    print_password_hash(password_hash);
    printf("\n>>> Using dictionary path: %s\n", dictionary_path);
  }

  // Open file
  char *dictionary_file_name = set_mpi_dictionary_filename(dictionary_path, my_rank);
  FILE *file = fopen(dictionary_file_name, "r");

  // Do comparison
  int result = compare_candidates(&file, password_hash, verbose, p);

  // propagate result to all processes
  int collective_result;
  MPI_Allreduce(&result, &collective_result, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  if (collective_result == NOT_FOUND && my_rank == 0)
    print_not_found(verbose);

  // Cleanup
  MPI_Finalize();
  fclose(file);
  free(dictionary_file_name);
  return 0;
}

/**
 * mpi_result_check() - a status check for the entire communicator
 *
 * Checks if another process has found the password match.
 * Allows the process that found the match to report its success.
 *
 * @param my_result is the status of the process that is reporting in. (FOUND or NOT_FOUND)
 * @return the result as an integer value, FOUND (0) or NOT_FOUND (1).
 */
int mpi_result_check(int my_result)
{
  int result_check;
  MPI_Allreduce(&my_result, &result_check, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  return result_check;
}

/**
 * set_mpi_dictionary_filename() - sets the name of the MPI dictionary filename for a given process
 *
 * @param dictionary_path is the directory where the split files are located/
 * @param rank of the process. This is needed for the proper file assignment and selection.
 * @return the filename of the dictionary generated and assigned for this process. Includes the full path.
 */
char *set_mpi_dictionary_filename(char *dictionary_path, int rank)
{
  int filename_length = strlen(dictionary_path) + 1;

  char *filename = (char *)malloc(sizeof(char) * (filename_length));
  if (rank < 10)
  {
    sprintf(filename, "%s/%s0%d", dictionary_path, FILE_PREFIX, rank);
  }
  else
  {
    sprintf(filename, "%s/%s%d", dictionary_path, FILE_PREFIX, rank);
  }
  return filename;
}

/**
 * compare_candidates() - comparing password_hash against hashed dictionary entires
 * (MPI Implementation)
 * 
 * 1. Manages iterating through the dictionary file and initiating the has comparisons.
 * 2. Returns the result value (FOUND or NOT_FOUND) and the plain text password, if found.
 *
 * @param file is a pointer to the dictionary file in memory.
 * @param password_hash is the hashed password string to crack.
 * @param verbose is a flag for verbose mode. Set to 1 to enable.
 * @param p is the number of MPI processees.
 * @return the result as an integer value, FOUND (0) or NOT_FOUND (1).
 */
int compare_candidates(FILE **file, char *password_hash, int verbose, int p)
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  // Flags and counters
  int count = 0;
  int result = NOT_FOUND;

  while ((read = getline(&line, &len, *file)) != -1)
  {
    char *candidate_buffer = NULL;
    remove_new_line(line, &candidate_buffer);

    // First check if it is already FOUND, and return if FOUND
    if (count == CHUNK_SIZE / p)
    {
      if (mpi_result_check(NOT_FOUND) == FOUND)
        return FOUND;

      count = 0;
    }

    int compResult = do_comparison(password_hash, candidate_buffer, verbose);
    count++;

    if (compResult == FOUND)
    {
      result = FOUND;
    }
  }
  return result;
}
