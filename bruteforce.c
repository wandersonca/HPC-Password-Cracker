#include <math.h>

int bruteforce_crack(char* password_hash, char * characters, int password_max_length, int verbose)
{
    printf("Brute force of hash: %s\n",password_hash);
    int number_of_characters = strlen(characters);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    static unsigned char buffer[65];
    printf("Calculating to a length of %d\n", password_max_length);

    int i,j,k;
    for(i=1; i <= password_max_length; i++) {
        long possibilities = (long) pow(number_of_characters, i);
        if(verbose)
        {
            printf("Now calculating password length of %d, it has %d possibilities\n", i, possibilities);
        }
        char passwordToTest[i];
        for (j = 0; j < possibilities; j++)
        {
            strcpy(passwordToTest, "");
            int val = j;
            for (k = 0; k < i; k++)
            {
                passwordToTest[k] = characters[val % number_of_characters];
                val = val / number_of_characters;       
            }
            hash(passwordToTest, buffer);
            if (!strcmp(password_hash, buffer))
            {
                printf("Password found: %s\n", passwordToTest);
                return 0;
            }       
        }
    }
    return 1;
}