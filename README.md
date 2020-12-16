# HPC Password Cracker
[![Build Status](https://cloud.drone.io/api/badges/wandersonca/HPC-Password-Cracker/status.svg)](https://cloud.drone.io/wandersonca/HPC-Password-Cracker)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Purpose
The HPC Password Cracker project is a password cracker that supports OpenMPI, Open MP technologies. Passwords values provided at the command line are expected to be hashed values using sha256sum encryption. 

## How to hash a password for testing
``echo -n 'test' | sha256sum``

## Compilation Notes
1. Be certain to install the libssl-dev package prior to compilation.
On Ubuntu: ``sudo apt-get install libssl-dev``

2. Run ``make clean`` between implementations to ensure the appropriate binaries are available for each individual implementation.

## Serial Implementation

### How to compile:
Run make: ``make serial``

### How to run dictionary attack:
Pass in the full path of the dictionary file.

``./bin/serial-cracker --dictionary dictionary_files/100_pass.txt --password $(echo -n '123456' | sha256sum) --verbose``

or using short option names:

``./bin/serial-cracker -d dictionary_files/100_pass.txt -p $(echo -n '123456' | sha256sum) -v``

### How to run brute force attack:
``./bin/serial-cracker --bruteforce --lowercase --uppercase --numbers --symbols -p $(echo -n 'test' | sha256sum) --verbose``

or using short option names:

``./bin/serial-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Parallel Implementation - MPI

### How to pre-split files for MPI dictionary attack:
Provide the number of lines per file. (_E.g._, With four processes and 100 passwords, we need four files. Divide the 100 passwords evenly into 25 per file.)

``mkdir -p temp && split -d -l 25 dictionary_files/100_pass.txt temp/file_``

Note: A current limitation here is the need to have a number of passwords even divisible by the number of processes.

### How to compile:
Run make: ``make mpi``

### How to run dictionary attack:
Pass in the "temp" directory for the location of the split files to be processed across processes.

``mpirun -np 4 ./bin/mpi-cracker -d temp -p $(echo -n '123456' | sha256sum) -v``

### How to run brute force attack:
``mpirun -np 4 ./bin/mpi-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Parallel Implementation - OpenMP

### How to compile:
Run make: ``make omp``

### How to run dictionary attack:
Pass in the full path of the dictionary file.
> Note: There is a known performance issue with the way OpenMP tasks are utilized here. Performance is not good. 

``OMP_NUM_THREADS=4 ./bin/omp-cracker -d dictionary_files/100_pass.txt -p $(echo -n '123456' | sha256sum) -v``

### How to run brute force attack:
``OMP_NUM_THREADS=4 ./bin/omp-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Parallel Implementation - CUDA
> Experemental: Only available for Brute Force attack. Also, in its current state, this solution is unoptimized and is not much faster than the serial version.

### How to compile:
Run make: ``make cuda``

### How to run brute force attack:
``./bin/cuda-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## Install MPI on Ubuntu
 Run from command line: ``sudo apt install libopenmpi-dev``

## Authors
* Donna Harris
* Ma Luo
* Will Anderson
