# CS50 Lab 6 - Tiny Search Engine Querier
## Luca Lit, CS50 Winter 2020
## Implementation.md

### Querier:

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### Usage

To use the querier module, enter in the commandline a syntax that matches the following: 
```
./querier pageDirectory indexFilename
```
* where pageDirectory is the pathname of a directory produced by the Crawler, and
* where indexFilename is the pathname of a file produced by the Indexer.

### Functional Decomposition

The querier module is implemented by querier.c, which contains the following functions: 

/**********************************functions**********************************/

```c
// Main function that takes in command line parameters
int main(int argc, char *argv[])

// Function that reads the query, cleans and parses and ranks the matching documents
int readandrank(FILE * fp, indx_t* indx, char * pageDirectory);

// function called by readandrank() which parses and cleans the query into a word array
int parse_clean(char * input, char * array[], int *actualarraysize);

// function that performs the intersection of two countersets
static void counters_intersect(void * countersetpair_t, const int doc_ID, int count);

// function that copies one counterset to another
static void countercopy(void * arg, const  int doc_ID, int count);

// function that sums up the counts between two countersets
static void union_(void * unionizedset, const int key, int count);

// Function used to count how many keys there are in the counterset
static void counting(void *t, const int doc_ID, int count);

// Ranking function that uses quicksort to rank document matches from decreasing order 
void ranker(counters_t * ctrset, int num_matches, char * pageDirectory);

// function for retrieval of URLs
char * returnURL(char * pageDirec, int doc_ID);

// Quicksort function
static void sort_array(docID_score_t *dIs [], int lo, int hi);

// Quicksort helper function
static int partitioning(docID_score_t *dIs[], int lo, int hi);

// Adds countersets to an array
static void ctrs_into_arr(void *arrPos, const int key, int c);

// Deleiting function for hashtable delete
static void KIdelete(void *i);

```

### Pseudocode of MAJOR FUNCTIONS 

MAIN:
1. reads commandline and validates parameters 
2. if the pagedirectory is invalid, return error message
3. Same if indexfilename is invalid 
4. Same is usage is wrong e.g. 5 arguments
5. loads index into index struct
6. calls readandrank()
7. Clean up

READANDRANK: 
1. first prompts for user query
2. reads line from stdin 
3. calls parse and clean to store it all in word array 
4. Goes through the whole array, and whenever there are ands, or no ands in between word, intersect them 
5. If there are or's encountered, store the intersetced set in a finalset (to be unioned later)
6. After reading all andsequences, union the array of finalsets into one unionized set 
7. Rank the unionized set using the ranker() function
8. If there are matching documents, print the ranked output
9. If not, print "no documents matching"
10. Prompt for another query and repeat

PARSEANDCLEAN:
1. Breaks the standard input into word array 
2. if there are ands and ors at the begining or ends of the array, return error message 
3. if there are ands and ors adjacent, print error messages


For more information on the implementation of the minor functions, refer to the extensive comments in querier.c. 


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

### Compilation

To compile, simply `make`.

To test, do `make test`.

To clean, do `make clean`.