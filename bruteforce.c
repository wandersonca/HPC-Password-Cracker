#include <math.h>

int bruteforce_crack(char* password_hash, char * characters, int password_max_length, int verbose)
{
    printf("Brute force of hash: %s\n",password_hash);
    int number_of_characters = strlen(characters);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    long possibilities = (long) pow(number_of_characters, password_max_length);
    printf("Number of possible words: %d\n", possibilities);
    static unsigned char buffer[65];
    if(verbose) 
    {
            printf("Hash to compare: %s\n", password_hash);
    }
    char passwordToTest[password_max_length];

    for (int i = 0; i < possibilities; i++)
    {
        strcpy(passwordToTest, "");
        int val = i;
        for (int j = 0; j < password_max_length; j++)
        {
            int ch = val % number_of_characters;
            passwordToTest[j] = characters[ch];
            val = val / number_of_characters;    
                           
        }
        hash(passwordToTest, buffer);
        if(verbose) 
        {
            printf("`%s` -> %s\n", passwordToTest, buffer);
        }

        if (!strcmp(password_hash, buffer))
        {
            printf("Password found: %s\n", passwordToTest);
            return 0;
        }       
    }
    return 1;
}