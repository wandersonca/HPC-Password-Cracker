int bruteforce_crack(char* password_hash, char * characters, int length, int verbose)
{
    printf("Brute force of hash: %s\n",password_hash);
    printf("Using %d of characters: %s\n", length, characters);

    static unsigned char buffer[65];
    if(verbose) 
    {
            printf("Hash to compare: %s\n", password_hash);
    }

    for(int i=0; i < 1; i++)
    {
        // we should get to this by brute force...
        char passwordToTest[] = "test";
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