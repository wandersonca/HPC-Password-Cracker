# HPC Password Cracker
[![Build Status](https://cloud.drone.io/api/badges/wandersonca/HPC-Password-Cracker/status.svg)](https://cloud.drone.io/wandersonca/HPC-Password-Cracker)

## Purpose
The HPC Password Cracker project is an educational exploration of various parallel implementations of two common password cracking approaches. Passwords values provided at the command line are expected to be hashed values using sha256sum encryption.

## How to hash a password for testing
``echo -n 'test' | sha256sum``

## Compilation Notes
1. Be certain to install the libssl-dev package prior to compilation.
On Ubuntu: ``sudo apt-get install libssl-dev``

2. Run ``make clean`` between implementations to ensure the appropriate binaries are available for each individual impelemtation.

## Serial Implementation

### How to compile:
Run make: ``make serial``

### How to run dictionary attack:
``./bin/serial-cracker --dictionary dictionary_files/100_pass.txt --password $(echo -n '123456' | sha256sum) --verbose``

or using short option names:

``./bin/serial-cracker -d dictionary_files/100_pass.txt -p $(echo -n '123456' | sha256sum) -v``

### How to run brute force attack:
``./bin/serial-cracker --bruteforce --lowercase --uppercase --numbers --symbols -p $(echo -n 'test' | sha256sum) --verbose``

or using short option names:

``./bin/serial-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Parallel Implementation - MPI

### How to pre-split files for parallel dictionary attack methods:
Provide the number of lines per file. (E.g., With 4 processes, we will need 4 files. 100 passwords split evenly into 25 lines per file.)

``split -d -l 25 dictionary_files/100_pass.txt temp/file_``

### How to compile:
Run make: ``make parallel``

### How to run dictionary attack:
Pass in the "temp" directory for the location of the split files to be processed across processes.

``mpirun -np 4 ./bin/mpi-cracker -d temp -p $(echo -n '123456' | sha256sum) -v``

### How to run brute force attack:
``mpirun -np 4 ./bin/mpi-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Parallel Implementation - OpenMP

### How to compile:
Run make: ``make omp``

### How to run dictionary attack:
Pass in the "temp" directory for the location of the split files to be processed across processes.

``gcc --fopenmp NUM_OF_THREADS=4 ./bin/mpi-cracker -d temp -p $(echo -n '123456' | sha256sum) -v``

### How to run brute force attack:
``gcc -fopenmp NUM_OF_THREADS=4 ./bin/mpi-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Authors
* Donna Harris
* Ma Luo
* Will Anderson
