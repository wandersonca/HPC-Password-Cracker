# HPC-Password-Cracker
[![Build Status](https://cloud.drone.io/api/badges/wandersonca/CP631-FinalProject/status.svg)](https://cloud.drone.io/wandersonca/CP631-FinalProject)

## How to hash a password for testing
``echo -n 'test' | sha256sum``

## How to compile
1. Make sure you install the libssl-dev package. On Ubuntu ``sudo apt-get install libssl-dev``.
2. Run gcc: ``gcc -std=c99 cli.c -o cli -lcrypto``

## How to compile with Docker
1. Run gcc within a container: ``docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp gcc:4.9 gcc -std=c99 cli.c -o cli -lcrypto``

## How to run dictionary attack:
``./cli --dictionary /some/path --password $(echo -n 'test' | sha256sum) --verbose``
or using short option names:
``./cli -d /some/path -p $(echo -n 'test' | sha256sum) -v``

## How to run brute force attack:
``./cli --bruteforce --lowercase --uppercase --numbers --symbols -p $(echo -n 'test' | sha256sum) --verbose``
or using short option names:
``./cli -bluns -p $(echo -n 'test' | sha256sum) -v``

## Authors
Donna Harris
Ma Luo
Will Anderson
