#include <math.h>

void print_stats(char *password_hash, char *characters, int number_of_characters, int password_max_length)
{
    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    printf("Calculating to a length of %d\n", password_max_length);
}

long calculate_possibilities(int number_of_characters, int length, int verbose)
{
    long possibilities = (long)pow(number_of_characters, length);
    if (verbose)
    {
        printf("Now calculating password length of %d, it has %ld possibilities\n", length, possibilities);
    }
    return possibilities;
}

void generate_password(int length, char *characters, int number_of_characters, int seed, char * result)
{
    int i;
    for (i = 0; i < length; i++)
    {
        result[i] = characters[seed % number_of_characters];
        seed = seed / number_of_characters;
    }
    result[length] = '\0';
}

int calculate_next_step(int counter, int possibilities, int chunk_size)
{
    if (counter + chunk_size > possibilities)
    {
        return possibilities;
    }
    else
    {
        return counter + chunk_size;
    }    
}