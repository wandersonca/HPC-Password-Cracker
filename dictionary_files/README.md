# About Dictionary Files

The included dictionary files are largely sourced from Daniel Miessler's "SecLists" GitHub repository. This repository is MIT licensed.

The files here have been renamed, as described below.

Other data files were also created to support smaller functional testing.

## Source of Sample Data

The GitHub repository for our dictionary files were sourced from [https://github.com/danielmiessler/SecLists/tree/master/Passwords](https://github.com/danielmiessler/SecLists/tree/master/Passwords).

Our provided test samples utilize the xato-net-10-million-passwords-<##>.txt files, where <##> represents a power of ten between 10 and 1 million.

**Caution:** The sourced dictionary files include some explicit language as it appears such language is a common choice for passwords.

### Renaming of Original Files

For ease of use at the command line, these files are renamed to that they are shorter and easier to work with.

| Sample Size | Original Name | Short Name of Sample |
| ------: | --------------|----------------------|
| 10 | xato-net-10-million-passwords-10.txt | 10_pass.txt |
| 100 | xato-net-10-million-passwords-100.txt | 100_pass.txt |
| 1000 | xato-net-10-million-passwords-1000.txt | 1k_pass.txt |
| 10000 | xato-net-10-million-passwords-10000.txt | 10k_pass.txt |
| 100000 | xato-net-10-million-passwords-100000.txt | 100k_pass.txt |
| 1 million | xato-net-10-million-passwords-1000000.txt | 1m_pass.txt |


## The "Ten Million Password" File

The purpose of a "Ten Million Password" file is to use it as the source data for the primary experiments run on the various implementations of the dictionary attack.

As of the time of this development effort, the file in the SecLists repository identified as having 10 million passwords (xato-net-10-million-passwords.txt) actually appears to have significantly less than 10 million passwords.

Instead of using that file for our biggest data experiments, the **10m_pass.txt** file was created and includes ten million passwords. The 10m_pass.txt file was created by concatenating ten replicated 1m_pass.txt files. Each replicate had a unique postfix value added to each password within the file in order to ensure the uniqueness of each replicated set. Finally, a few passwords were manually added to round out the collection, thereby arriving at ten million passwords.
