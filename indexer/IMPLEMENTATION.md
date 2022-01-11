# CS50 Lab 4 - IMPLEMENTATION.md
## Luca Lit, CS50 Winter 2020

### Functions

My indexer and indextest is implemented through the following functions and modules (listed only the relevant ones):

```c
// From indexer.c 

int main(int argc, char *argv[]);

index_t* buildinvertedindex(char* pageDirectory, index_t* indx);

static void KIdelete(void* i);

// From indextest.c

int main (int argc, char *argv[]);

// From index.h

typedef hashtable_t indx_t;

indx_t *indx_new(const int num_of_slots);

bool indx_insert(indx_t *indx, char *key, counters_t *counts);

counters_t* indx_find(indx_t* indx, char* key);

void index_saver(hashtable_t* indx, char* nameoffile);

void key_saver(void *arg, const char *key, void *i);

void count_saver(void *arg, const int key, int count);

hashtable_t index_loader(char * nameoffile, hashtable_t * indx);

// From word.h

char * NormalizeWord(char * string);

// From pagedir.h

int isCrawlerDirectory(char *dir);

```

To explore more in depth of how each function was implemented, read through the following pseudocodes:

```c
int main(int argc, char *argv[]);
```
- The main function takes in as parameters argc (which is for counting how many arguments are supplied), and a string array argv[], which we later use  to refer to which argument index
- The type is of int, so if everything runs successfully, ends with 0 status, else, non zero status.

- The function first verifies whether the correct number of arguments is entered / whether usage was correct:
	- The number of arguments supplied must be equal to 2 (in addition to the ./indexer)
	- The function also checks whether the directory entered was created by Crawler.
	- The function checks whether the file name exists and can be read

- Once all the above conditions are fulfilled, it will begin building the inverted index.
- the index file is then saved by calling the index_saver() function from index.h
- and finally, to clean up, we call hashtable_delete.

```c
hashtable_t* buildinvertedindex(char* pageDirectory, hashtable_t* indx);
```

- The buildinvertedindex function takes two parameters, a valid page directory (filename) and a hashtable
- the type is of indx_t, which is effectively a hashtable_t as defined in index.h

- The function reads documents from the pageDirectory provided by the user.
  Note: each has a unique ID in which the ID starts at 1 and increments by 1 for every new page;

- While there are files in the pageDirectory to be read:
	- if the file is open and readable, extract URL, DEPTH and HTML data from the file.
	- if the file's first linie is not an URL (i.e. invalid file), then continue to the next iteration of the loop.
	- Once extracted, all this is stored into/with a webpage struct

- Next, the function reads and extracts words from the HTML, making use of the webpage.h module, finds the number of occurences of that word (saving it using a counterset) and inserts it into the index struct using indx_insert().

	- Note that the word must not have fewer than 3 characters, and it must be Normalised too (meaning converted to only lower case letters)

- To clean up, free the memory that was allocated.
	

### Major Data Structures

- Hashtable - implemented through hashtable.c in libcs50 folder
  -> refer to hashtable.h for more details

- Set - implemented through set.c in libcs50 folder
  -> indirectly used through hashtable - refer to set.h for more details

- Counter - implemented throuogh counters.c in libcs50 folder
  -> refer to counters.h for more details

### Functional Decomposition into modules 

The indexer and indextest makes use of pagedir.c, index.c and word.c. Their functions are also listed above at the top of the document. To understand more about how each one was implemented, refer to their header files, which exist in the /common/ directory.

### Assumptions while implementing

The indexer makes the following assumptions:

1. pageDirectory (first parameter) has files named 1, 2, 3, …, without gaps.

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

No further assumptions other than ones specified in instructions.

### Error Handling: 

The following are cases where there are errors, and the exit statuses of each when they happen: 

Incorrect number of arguments: exit(1)
Not a Directory: exit(2)
Not a Crawler-produced directory: exit(3)

If everything runs successfully, return 0  status.
