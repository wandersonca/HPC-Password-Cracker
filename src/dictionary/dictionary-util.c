#include "../hash/hash.h"
#include "../globals.h"

/**
 * remove_new_line() - checks for the new line character, removes if found
 *
 * @param input is a string that might have a new line character at the end.
 * @return ouput, a string with no newline character at the end.
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

/**
 * print_password_found() - pretty print of the provided password hash
 *
 * @param password_hash is the hash of the password the program is looking for.
 */
void print_password_hash(char *password_hash)
{
    printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Looking for this password hash:\t\t\t\t\t%s\n", password_hash);
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
}

/**
 * print_password_found() - print the found password
 * 
 * Verbose mode enables pretty print of the statement.
 *
 * @param match is the plain text password to be printed.
 * @param verbose is a flag for verbose mode. Set to 1 to enable.
 */
void print_password_found(char *match, int verbose)
{
    if(verbose)
        printf("\n>>> SUCCESS!! ");

    printf("Password found: %s\n", match);

    if(verbose)
        printf("\n");
}

/**
 * print_not_found() - prints a statement that a match was not found
 * 
 * Verbose mode enables pretty print of the statement.
 *
 * @param verbose is a flag for verbose mode. Set to 1 to enable.
 */
void print_not_found(int verbose)
{
    if(verbose)
        printf("\n >>> ");
        
    printf("Password not found.\n");

    if(verbose)
        printf("\n");
}

/**
 * do_comparison() - compare two hashed values
 *
 * @param password_hash is the hashed password string to crack.
 * @param candidate_buffer is the plain text password candidate from the dictionary file.
 * @param verbose is a flag for verbose mode. Set to 1 to enable.
 * @return the result as an integer value, FOUND (0) or NOT_FOUND (1).
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