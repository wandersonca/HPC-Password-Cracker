#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <omp.h>
#include "../hash/hash.h"
#include "dictionary-util.c"

int dictionary_crack(char* password_hash, char *dictionary_path, int verbose)
{
    FILE *file;
    char *line = NULL;
	size_t len = 0;
	ssize_t read;
 
	file = fopen(dictionary_path, "r");
	if (file == NULL)
    {
        printf("Error reading dictionary file: %s\n", dictionary_path);
        printf("Exiting with error code %d.\n", EXIT_FAILURE);
		exit(EXIT_FAILURE);
    }

    printf("\n>>> Using dictionary path: %s\n\n", dictionary_path);

    //hash(password_hash, password_buffer);
    if(verbose) 
    {
        printf("---------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Looking for this password:\t\t\t\t\t%s\n", password_hash);
        printf("---------------------------------------------------------------------------------------------------------------------------------\n");
        printf("\n");
    }

    int result =1;

    #pragma omp parallel
    #pragma omp single
  	while ((read = getline(&line, &len, file)) != -1)
    {
        char *candidate_buffer = NULL;
        remove_new_line(line, &candidate_buffer);   

        #pragma omp task firstprivate(candidate_buffer)
        {
            unsigned char candidate_hash[65];
            hash(candidate_buffer, candidate_hash); 

            if(verbose) 
            {
                printf("Password candidate from file:\t%16s\t--->\t%s\n", candidate_buffer, candidate_hash);
            }

            if (!strcmp(password_hash, candidate_hash))
            {
                printf("\nSUCCESS!!\tPassword found: %s\n", candidate_buffer);
                result = 0;
            }        

            free(candidate_buffer);
        }
    }

    fclose(file);

    printf("\n");

    if (result)
    {
        printf("Password not found.\n");
    }

    return result;
}