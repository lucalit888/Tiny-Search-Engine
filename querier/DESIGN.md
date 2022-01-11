# CS50 Lab 6 DESIGN.md - for querier
## DESIGN SPEC

### User interface (INPUT)

The User interface is simply the commandline, and it must accept 2 arguments as parameters. The Usage is as follows:

./querier pageDirectory indexfilename

After the parameters are verified, i.e. pageDirectory exists and is produced by Crawler, and indexfilename is a file produced by the indexer, then the user is prompted for standard input, until ctrl + D is inputted to express end of file.

### OUTPUT

The querier reads the standard input.

It then outputs the following: 
- if there are matching documents to the user input (calculate through the ranking() function), then the output mirrors as follows:
```

Query? dartmouth or computer science or programming or unix                
Query: dartmouth or computer science or programming or unix 
Matches 7 documents (ranked):
score 422 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score 222 doc   5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
score 195 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score  93 doc   6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score  13 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score  10 doc   3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   4 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
```

- If there are no matching documents, then the result is "No matching documents found"

- For error cases, it will print the corresponding error messages

### Functional module decomposition

The following MAJOR functions and modules will be used in querier.c 

MAIN (type int): 

- A function that checks whether the correct number of arguments were inputted, and whether the directory is valid and is produced by the crawler. It also checks whether the indexfilename exists and is readable If not, it exits with non zero status. 

- If the usage is correct, it will first load the index, before calling a function to read and rank the documents


READ AND RANK:

- This function will read the standard input, put it into a word array, and then calculate the scores. 
- Score calculation happens by first reading and breaking up the array into and sequences. 
- Whenever an or is encountered, it will add everything before the 'or', into a final array. 
- When all the intersections of sets are complete, the program proceeds to union them, which is summing up all the scroes between them, separated by the or statements. 
- Finally, we will rank the documents. 

To accompany that, the major data structures that are used include: 

1. Hashtable
2. Counter 
3. Set

To find out more about each individual one, please refer to their header files, which can be found in libcs50 file/ 

### Pseudocode:

In short, this is what the pseudocode for querier would look like: 

Querier:

1. Checks validity of arguments
2. Loading index
3. Read user query from stdin
4. Parse and Clean query into a word array (empty spaces ignored)
5. if the query is not of correct format, return error message 
6. Calculates the scores of and sequences 
7. Unions those scores to make a final counterset
8. Ranks the final counterset's documents by their score number
9. Exit program with 0 status if successful.


### Assumptions

The querier may assume that the input directory and files follow the designated formats.

* pageDirectory has files named 1, 2, 3, …, without gaps.

* The content of files in pageDirectory follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.

* The content of the file named by indexFilename follows our Index file format; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.


### Exit Status

```
Status | Corresponds to
------------------------------------------------
   0   | Execution successful 
   1   | Incorrect number of arguments inputted
   2   | pageDirectory inputted does not exist
   3   | pageDirectory inputted was not produced by Crawler
   4   | indexFileName inputted does not exist or is not readable
```


