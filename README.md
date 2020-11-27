# HPC Password Cracker
[![Build Status](https://cloud.drone.io/api/badges/wandersonca/HPC-Password-Cracker/status.svg)](https://cloud.drone.io/wandersonca/HPC-Password-Cracker)

## How to hash a password for testing
``echo -n 'test' | sha256sum``

## How to compile
1. Make sure you install the libssl-dev package. On Ubuntu ``sudo apt-get install libssl-dev``.
2. Run make: ``make serial``

## How to run dictionary attack:
``./bin/serial-cracker --dictionary dictionary_files/100_pass.txt --password $(echo -n '123456' | sha256sum) --verbose``

or using short option names:

``./bin/serial-cracker -d dictionary_files/100_pass.txt -p $(echo -n '123456' | sha256sum) -v``

## How to run brute force attack:
``./bin/serial-cracker --bruteforce --lowercase --uppercase --numbers --symbols -p $(echo -n 'test' | sha256sum) --verbose``

or using short option names:

``./bin/serial-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

## How to run MPI version:
``mpirun -np 4 ./bin/mpi-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``
``mpirun -np 4 ./bin/mpi-cracker -d dictionary_files/100_pass.txt -p $(echo -n '123456' | sha256sum) -v``

## Authors
* Donna Harris
* Ma Luo
* Will Anderson
