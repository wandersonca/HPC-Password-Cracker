#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "dictionary/dictionary.h"
#include "bruteforce/bruteforce.h"

void help();

/**
 * main() - HPC Password Cracker
 *
 * Launches the HPC Password Cracker program
 * 
 * @return An integer value aligning with FOUND (0) and NOT_FOUND (1) results from the cracker execution.
 */
int main(int argc, char **argv)
{
    int c;
    // Flags
    int verbose_flag = 0;
    int bruteforce_flag = 0;
    int dictionary_flag = 0;
    int character_length = 4;
    char password_hash[65] = "";
    char bruteforce_characters[256] = "";
    char dictionary_path[256] = "";

    while (1)
    {
        // Used for parsing long options (--long-option)
        static struct option long_options[] =
            {
                {"verbose", no_argument, 0, 'v'},
                {"password", required_argument, 0, 'p'},
                {"dictionary", required_argument, 0, 'd'},
                {"bruteforce", no_argument, 0, 'b'},
                {"uppercase", no_argument, 0, 'u'},
                {"lowercase", no_argument, 0, 'l'},
                {"numbers", no_argument, 0, 'n'},
                {"symbols", optional_argument, 0, 's'},
                {"character-length", required_argument, 0, 'c'},
                {"help", no_argument, 0, 'h'}
            };

        // getopt_long stores the option index here
        int option_index = 0;

        c = getopt_long(argc, argv, "hvp:d:bulns::c:", long_options, &option_index);

        // Detect the end of the options
        if (c == -1)
            break;

        switch (c)
        {
            case 'h':
                help();
                return(0);
            case 'v':
                verbose_flag=1;
                break;
            case 'p':
                strcpy(password_hash,optarg);
                break;
            case 'd':
                dictionary_flag=1;
                strcpy(dictionary_path,optarg);
                break;
            case 'b':
                bruteforce_flag=1;
                break;
            case 'u':
                strcat(bruteforce_characters, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                break;
            case 'l':
                strcat(bruteforce_characters, "abcdefghijklmnopqrstuvwxyz");
                break;
            case 'n':
                strcat(bruteforce_characters, "0123456789");
                break;
            case 's':
                if(optarg == NULL)
                {
                    strcat(bruteforce_characters, "~!@#$%^&*()_+");
                } 
                else 
                {
                    strcat(bruteforce_characters, optarg);
                }
                break;
            case 'c':
                character_length = atoi(optarg);
                break;
            case '?':
                help();
                break;

            default:
                abort();
        }
    }

    int result;

    // Ensure only one mode is selected
    if(dictionary_flag ^ bruteforce_flag)
    {
        if(dictionary_flag)
        {
            result = dictionary_crack(password_hash, dictionary_path, verbose_flag);
        }

        if(bruteforce_flag) 
        {
            result = bruteforce_crack(password_hash, bruteforce_characters, character_length, verbose_flag);
        }
    }
    else
    {
        
        printf("\n>>> Invalid attack selection. Must choose either dictionary or brute force attack.\n");
        help();
        result = 1;
    }
    
    exit(result);
}





/**
 * help() - Print out of the HPC Password Cracker Help guidance
 *
 * The HPC Password Cracker Help provides instructions and guidance for:
 * - General Usage
 * - Dictionary Attack Usage
 * - Brute Force Attack Usage
 */
void help()
{
    printf("\n__HPC Password Cracker Help__\n\n");

    printf("General Usage:\n");
    printf("   Run Serial:\t\t\t serial-cracker [attack_type] [attack_settings] -p [password_hash]\n");
    printf("   Run Parallel using MPI:\t mpi-cracker -np [number_of_processes] [attack_type] [attack_settings] -p [password_hash]\n");
    printf("   Run Parallel using OpenMP:\t omp-cracker -fopenmp OMP_NUM_THREADS=[number_of_threads] [attack_type] [attack_settings] -p [password_hash]\n");

    printf("\n");    
    
    printf("\"Dictionary Attack\" Usage:\n");
    printf("   Run Serial:\t\t\t serial-cracker -d [path] -p [password_hash]\n");
    printf("   Run Parallel using MPI:\t mpi-cracker -np [number_of_processes] -d [path] -p [password_hash]\n");
    printf("   Run Parallel using OpenMP:\t omp-cracker -fopenmp OMP_NUM_THREADS=[number_of_threads] -d [path] -p [password_hash]\n");

    printf("\n");

    printf("\"Brute Force Attack\" Usage:\n");
    printf("   Run Serial:\t\t\t serial-cracker -b [character_arguments] -p [password_hash]\n");
    printf("   Run Parallel using MPI:\t mpi-cracker -np [number_of_processes] -b [character_arguments] -p [password_hash]\n");
    printf("   Run Parallel using OpenMP:\t omp-cracker -fopenmp OMP_NUM_THREADS=[number_of_threads] -b [character_arguments] -p [password_hash]\n");

    printf("\nArguments:\n");
    printf("   -p <hash>\t(or:  --password)\tThe hashed password value\n");
    printf("   -d <path>\t(or:  --dictionary)\tRun a Dictionary Attack using file located at <path>\n");
    printf("   -b\t\t(or:  --bruteforce)\tRun a Bruce Force Attack\n");
    printf("   -l\t\t(or:  --lowercase)\tIncludes lowercase letters in brute force attack\n");
    printf("   -u\t\t(or:  --uppercase)\tIncludes uppercase letters in brute force attack\n");
    printf("   -n\t\t(or:  --numbers)\tIncludes numerical digits in brute force attack\n");
    printf("   -s\t\t(or:  --symbols)\tIncludes symbol characters in brute force attack\n");
    printf("   -v\t\t(or:  --verbose)\tBe verbose with output (Recommended for debugging small data only)\n");
    printf("   -h\t\t(or:  --help)\t\tPrint Help (this message) and exit\n");

    printf("\n");

}