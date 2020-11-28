# About Dictionary Files

The included dictionary files are largely sourced from Daniel Miessler's "SecLists" GitHub repository. This repository is MIT licensed.

These files have been renamed, as described below.

Other data files were created for smaller functional testing.

## Source of Sample Data

The GitHub repository for our dictionary files were sourced from [https://github.com/danielmiessler/SecLists/tree/master/Passwords](https://github.com/danielmiessler/SecLists/tree/master/Passwords).

Our provided test sampels utilize the xato-net-10-million-passwords-<##>.txt files, where <##> represents a power of ten between 10 and 1 million.

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
| 10 million | xato-net-10-million-passwords.txt | 10m_pass.txt |

