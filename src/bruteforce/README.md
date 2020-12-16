# About the Brute Force Attack

For general compilation or execution details, please see README.md in the root project folder.

## Execution Details

The pattern used on the command line is identical for all cracker applications, regardless of the implementation approach.

### Set the Mode

The brute force attack mode is specified by using either a **-b** or **--bruteforce** argument followed by arguments relating to the allowed character types.

### Set the Options

The options include lowercase and uppercase letters, numeric digits, and symbols. Any or all of these may be used as options for the execution of a brute force attack. 

The settings may be specified as follows:

| Include: | Short Argument | Long Argument |
| ------ | --------------|----------------------|
| lowercase letters | -l | --lowercase |
| uppercase letters | -u | --uppercase |
| numeric digits | -n | --numbers |
| symbol characters | -s | --symbols |

### Set the Password

All program runs require the hashed password to be specified using the -p argument followed by the hashed password string. The approach modelled in our project documentation generates the hash for a plain text password: 

-p $(echo -n 'test' | sha256sum) 

### Sample test case overall

Under root folder of git project, clean solution: ``make clean``

For serial: ``make serial || ./bin/serial-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

For MPI: ``make mpi || mpirun -np 4 ./bin/mpi-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``

For OMP: ``make omp || OMP_NUM_THREADS=4 ./bin/omp-cracker -bluns -p $(echo -n 'test' | sha256sum) -v``