

/*
    remove_new_line()
    - utility function to remove new line character from each entry in the given file 
*/
int remove_new_line(char *input, char **output)
{
    int len = strlen(input);

    if(input[len-1] == '\n')
    {
        input[len-1] = '\0';
        *output = (char*)malloc(sizeof(char)*(len));
        strncpy(*output, input, len);
    }
    else
    {
        *output = (char*)malloc(sizeof(char)*(len+1));
        sprintf(*output, "%s\0", input);
    }
    
    return 0;
}

void print_password_hash(char *password_hash)
{
    printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Looking for this password hash:\t\t\t\t\t%s\n", password_hash);
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
}

void open_dictionary_file(char *dictionary_path, FILE **file)
{
	*file = fopen(dictionary_path, "r");
    if (file == NULL)
    {
        printf("Error reading dictionary file: %s\n", dictionary_path);
        printf("Exiting with error code %d.\n", EXIT_FAILURE);
		exit(EXIT_FAILURE);
    }
}

void close_dictionary_file(FILE **file)
{
    fclose(*file);
}


void compare_candidates(FILE **file, char *password_hash, int verbose, int *result)
{
    static unsigned char candidate_hash[64];
    char *line = NULL;
	size_t len = 0;
	ssize_t read;

  	while ((read = getline(&line, &len, *file)) != -1)
    {
        char *candidate_buffer = NULL;

        remove_new_line(line, &candidate_buffer);        
        hash(candidate_buffer, candidate_hash);

        if(verbose) 
            printf("Password candidate from file:\t%16s\t--->\t%s\n", candidate_buffer, candidate_hash);

        if (!strcmp(password_hash, candidate_hash))
        {
            printf("\nSUCCESS!!\tPassword found: %s\n", candidate_buffer);
            *result = 0;  /* a match was found */

            free(candidate_buffer);  
            return;  
        }        

        free(candidate_buffer);    
    }
}