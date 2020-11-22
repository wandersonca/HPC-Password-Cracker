#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "hash.c"

int remove_new_line(char *input, char **output)
{
    int len = strlen(input);

    if(input[len-1] == '\n')
    {
        *output = (char*)malloc(sizeof(char)*(len-1));
        strncpy(*output, input, len-1);
    }
    else
    {
        *output = (char*)malloc(sizeof(char)*(len-1));
        strncpy(*output, input, len);
    }
    
    return 0;
}

int dictionary_crack(char* password_hash, char *dictionary_path, int verbose)
{
    static unsigned char candidate_buffer[64];
    static unsigned char password_buffer[64];

    FILE *file; 
    char *line = NULL;
	char *password_to_test = NULL;
	size_t len = 0;
	ssize_t read;
 
	file = fopen(dictionary_path, "r");
	if (file == NULL)
		exit(EXIT_FAILURE);

    printf("\n>>> Using dictionary path: %s\n\n", dictionary_path);

    hash(password_hash, password_buffer);
    if(verbose) 
    {
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Looking for this password:\t%16s\t--->\t%s\n", password_hash, password_buffer);
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("\n");
    }

  	while ((read = getline(&line, &len, file)) != -1)
    {
        remove_new_line(line, &password_to_test);        
        hash(password_to_test, candidate_buffer);

        if(verbose) 
        {
            printf("Password candidate from file:\t%16s\t--->\t%s\n", password_to_test, candidate_buffer);
        }

//        if (!strcmp(password_hash, buffer))
        if (!strncmp(password_buffer, candidate_buffer, 64))
        {
            printf("\nSUCCESS!!\tPassword found: %s\n", password_to_test);
            free(password_to_test);
	        fclose(file);
            return 0;
            // TODO: What do we really need to return here? Do we need to return the matching value?
        }        

        free(password_to_test);        
    }

    fclose(file);

    printf("\n");

    return 1;
}