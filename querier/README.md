# CS50 Lab 6 - Tiny Search Engine Querier
## Luca Lit, CS50 Winter 2020

### Crawler:

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
int main(int argc, char *argv[])

int readandrank(FILE * fp, indx_t* indx, char * pageDirectory);

int parse_clean(char * input, char * array[], int *actualarraysize);

static void counters_intersect(void * countersetpair_t, const int doc_ID, int count);

static void countercopy(void * arg, const  int doc_ID, int count);

static void union_(void * unionizedset, const int key, int count);

static void counting(void *t, const int doc_ID, int count);

void ranker(counters_t * ctrset, int num_matches, char * pageDirectory);

char * returnURL(char * pageDirec, int doc_ID);

static void sort_array(docID_score_t *dIs [], int lo, int hi);

static int partitioning(docID_score_t *dIs[], int lo, int hi);

static void ctrs_into_arr(void *arrPos, const int key, int c);

static void KIdelete(void *i);

```

For more information on each of the functions, refer to IMPLEMENTATION.md

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

