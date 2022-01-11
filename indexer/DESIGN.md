# CS50 Lab 5 DESIGN.md - for indexer

## DESIGN SPEC

### User interface (INPUT)

The User interface is simply the commandline, and it must accept 2 arguments as parameters. The Usage is as follows:

./indexer pageDirectory filename

### OUTPUT

The indexer reads files that are produced by the Crawler, and then from there, it will build an inverted index, and saves it to a file. 

Our index maps from words to (docID, count) pairs, representing the number of occurences of that word in the document. To implemenet an index, we use a hashtable in which each item is a counterset where we use docID as key. 

On the other hand, in indextester, we will load an index file that the indexer has produced, and we proceed to build an index structure from that file, and saves it to another file. 

In testing.sh, we will test to see if there are any differences between the two testingfiles. 

### Functional module decomposition

The following functions and modules will be used in indexer.c"

MAIN (type int): 

- A function that checks whether the correct number of arguments were inputted, and whether the directory is valid and is produced by the crawler. If not, it exits with non zero status. 

- If the usage is correct, it will called a index builder function, which will read the caller-provided directory. 

- After building the index structure, we will call a index saving function to save it to a file that can be verified.


INDEX BUILDING (type index struct - which is effectively a hashtable):

- This function will build the index, and is called by main.
- As long as there are files to be read inside the directory, we will use the file.h functions to read into every file, save the first line as the URL, second line as the Depth and third line as the HTML, and stores all this in a webpage struct 
- Then, build an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. Ignore words with fewer than three characters, and “normalize” the word before indexing. Here, “normalize” means to convert all letters to lower-case.

To accompany that, the major data structures that are used include: 

1. Hashtable
2. Counter 
3. Set

To find out more about each individual one, please refer to their header files, which can be found in libcs50 file/ 

### Pseudocode:

In short, this is what the pseudocode for indexer and indextest would look like: 

Indexer:

1. Process and validate command-line parameters
2. Initialize data structure index
3. index_build(directory, index)
4. index_save(file, index);
5. clean up data structures

indextest:

1. Process and validate command-line parameters
2. Initialize data structure index
3. index_load(file1, index)
4. index_save(file2, index)
5. clean up data structures


### Major data structures

Two helper modules provide data structures:

 1. *index* of (word -> counters) structures
 2. *counters* of (docID,count) structures

### Assumptions:

The indexer may assume that:

1. pageDirectory has files named 1, 2, 3, …, without gaps.

2. The content of files in pageDirectory follow the format as defined in the specs; thus your code to read the files need not have extensive error checking.

3. The indexer writes the inverted index to a file, and both the index tester and the querier read the inverted index from a file; the file shall be in the following format.

one line per word, one word per line
each line provides the word and one or more (docID, count) pairs, in the format
word docID count [docID count]…
where word is a string of lower-case letters,
where docID is a positive non-zero integer,
where count is a positive non-zero integer,
where the word and integers are separated by spaces.
Within the file, the lines may be in any order.

Within a line, the docIDs may be in any order.

The index tester may assume that:

The content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

### Error Handling: 

Incorrect number of arguments: exit(1)
Not a Directory: exit(2)
Not a Crawler-produced directory: exit(3)

If everything runs successfully, return 0  status.


