#include "../hash/hash.h"
#include "../globals.h"

/* 
    remove_new_line()
        - checks for the new line character, removes if found

        input:  a string that might have a new line character at the end
        ouput:  a string with no newline character at the end
*/
void remove_new_line(char *input, char **output)
{
    int len = strlen(input);

    if (input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
        *output = (char *)malloc(sizeof(char) * len);
        strncpy(*output, input, len);
    }
    else
    {
        *output = (char *)malloc(sizeof(char) * (len + 1));
        sprintf(*output, "%s", input);
    }
}

/* 
    print_password_hash()
        - pretty print of the provided password hash

        password_hash:    the hash of the password the program is looking for
*/
void print_password_hash(char *password_hash)
{
    printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Looking for this password hash:\t\t\t\t\t%s\n", password_hash);
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
}

/* 
    print_password_found()
        - print the found password; verbose mode enables pretty print

        match:     the plain text password to be printed
        verbose:   set to 1 for verbose mode
*/
void print_password_found(char *match, int verbose)
{
    if(verbose)
        printf("\n>>> SUCCESS!! ");

    printf("Password found: %s\n", match);

    if(verbose)
        printf("\n");
}

/* 
    print_not_found()
        - print statement that match was not found; verbose mode enables pretty print

        verbose:   set to 1 for verbose mode
*/
void print_not_found(int verbose)
{
    if(verbose)
        printf("\n >>> ");
        
    printf("Password not found.\n");

    if(verbose)
        printf("\n");
}

/* 
    do_comparison()
        - 1. hashes the plaintext password candidate from the dictionary file
        - 2. compares two hashed values
        - 3. returns the result value (FOUND or NOT_FOUND) and the plain text password, if found

        password_hash:      hashed value of the password to be cracked
        candidate_buffer:   plain text password candidate from the dictionary file
        verbose:            set to 1 for verbose mode

*/
int do_comparison(char *password_hash, char *candidate_buffer, int verbose)
{
    unsigned char candidate_hash[65];
    hash(candidate_buffer, candidate_hash);

    if (DEBUG)
        printf("Password candidate from file:\t%16s\t--->\t%s\n", candidate_buffer, candidate_hash);   

    if (!strcmp(password_hash, candidate_hash))
    {
        print_password_found(candidate_buffer, verbose);
        return FOUND;
    }
    return NOT_FOUND;
}