# CS50 Lab 4 - common
## Luca Lit, CS50 Winter 2020

### pagedir

pagedir contains a collection of utility functions that is used for fetching, saving, and scanning web pages, and also an integer to string converter. It is designed to be for use by the crawler.c program. 

### index

index.c contains a collection of utlity functions that is used for loading and saving index files. It is used by the indexer.c and indextest.c programs. More information detailed below. 

### word 

word.c contains the utility function for converting a string to lowercase letters. Read more about the function in word.h. 

### Usage

The pagedir module, defined in `pagedir.h` and implemented in `pagedir.c`, implements the following functions: 

```c
// checks whether dir is a directory produced by the crawler.
int isCrawlerDirectory(char *dir);

// pagefetcher function which simply calls webpage_fetch from webpage.h, and returns a boolean to tell if fetch was successful
bool pagefetcher(webpage_t *page);

// page saver function which takes in a page, a path and an unique ID as parameters. 
int pagesaver(webpage_t *page, char * path, int id);

// a pagescanner function of type int which parses the caller-provided webpage to extract all its embedded URLs
int pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag);

// A function called itoa - which converts integer to string - takes in the integer as the parameter - used in my pagesaver function to create the file path 
char* itoa(int integer);
```

The index module, defined in `index.h` and implemented in `index.c`, implements the following functions: 

```c
indx_t *indx_new(const int num_of_slots);

// Function that inserts a counterset, given an index and a key
// since hashtable insert returns bool, our func type is also bool
bool indx_insert(indx_t *indx, char *key, counters_t *counts);

// Function that takesin an index struct and a key, and returns the counterset that hashtable_find() finds
counters_t* indx_find(indx_t* indx, char* key);

// index_saver function which calls hashtable iterate and applies the item_saver function 
void index_saver(hashtable_t* indx, char* nameoffile);

// function which makes use of counter's iterate function and applies count_saver
void key_saver(void *arg, const char *key, void *i);

// function that prints item to the file fp in the form (key : count)
void count_saver(void *arg, const int key, int count);

// function that loads an index file produced by the indexer
hashtable_t* index_loader(char * nameoffile, hashtable_t * indx)

```

### Implementation

Refer to pagedir.h, index.h, word.h and ../indexer/IMPLEMENTATION.md for information on how each of the functions were implemented.

### Assumptions

Assumptions as described in the assignment: 

1. pageDirectory has files named 1, 2, 3, …, without gaps.
2. The content of files in pageDirectory follow the format as defined in the specs; thus your code to read the files need not have extensive error checking.

3. Index file format

The indexer writes the inverted index to a file, and both the index tester and the querier read the inverted index from a file; the file shall be in the following format.

one line per word, one word per line
each line provides the word and one or more (docID, count) pairs, in the format
word docID count [docID count]…
where word is a string of lower-case letters,
where docID is a positive non-zero integer,
where count is a positive non-zero integer,
where the word and integers are separated by spaces.
Within the file, the lines may be in any order.

Within a line, the docIDs may be in any order.

No further assumptions other than specified by the instructions. 

### Compilation

To compile, simply `make`.

To clean, do `make clean`.

