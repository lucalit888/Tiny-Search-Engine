# CS50 Lab 5 - Indexer - TESTING.md
## Luca Lit - 17 Feb 2020

### TEST CASES:

This markdown file describes what test cases were executed in the testing.sh file. 


The tests are as follows:

-> TEST CASES THAT SHOULD FAIL: 

1) Test result for invalid number of arguments supplied by user
-  expect as a result: "Incorrect number of arguments inputted. Usage: ./indexer pageDirectory indexFilename."

2) Testing incorrect inputs (i.e. invalid directory)
- expect as a result: "Invalid directory. Please provide valid one."

3) Testing incorrect inputs (i.e. directory not produced by crawler)
- expect as a result: "Directory not produced by Crawler"


-> TEST CASES THAT SHOULD PASS:

4) Testing a correct example on the following directory '~cs50/data/tse-output/letters-depth-1'

	(a) Testing the indexer on  ~cs50/data/tse-output/letters-depth-1 + testingfile1  

	- expect test to look like:./indexer ~cs50/data/tse-output/letters-depth-4 testingfile1

	- Then cat to see what the index struct looks like 

	(b) Testing the indextest on testingfile1 and generating testingfile2

	- expect test to look like:./indextest testingfile1 testingfile2

	- Then cat to see what both index struct looks like 

	(c) See the differences between the two files 
	- orders the indexfile, then compares to see if there are any differences 
	- there should be no differences. 

Note: the directories '~cs50/data/tse-output/toscrape-depth-0' and '~cs50/data/tse-output/wikipedia-depth-0' were also tested, and successfully passed. However, they were excluded from the testing.sh file simply because they took too long for testing. 

The output of these files are directed into the testing.out file. 

Valgrind to check for memory leaks.	