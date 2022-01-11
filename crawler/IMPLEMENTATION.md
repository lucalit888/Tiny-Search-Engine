# CS50 Lab 4 - IMPLEMENTATION.md for TSE crawler 
## Luca Lit, CS50 Winter 2020

### This markdown file details the implementation specifications for the crawler program that I wrote. It contains pseudocode for each of the functions of my code, describes abstract data structures, defines APIs / interfaces / functions prototypes & parameters, data structures and error handling + recovery. 

### Function of the crawler

The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded href URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to maxDepth hops from the seed URL. When the crawler process is complete, the indexing of the collected documents can begin.

### User interface
The crawler’s only interface with the user is on the command-line; it must always be provided 3 arguments, like in the following example: 

crawler seedURL pageDirectory maxDepth

$ crawler http://old-www.cs.dartmouth.edu/index.html data 2

### Inputs / Outputs
Input: explained above.

Output: We save each explored webpage to a file, one file per page. The filename is the unique ID (assigned by the number of pages visited), for IDs 1, 2, 3, 4, and so forth. Within a file, it contains: 

- the page URL on the first line. 
- the depth of the page (where the seed is depth 0) on the second line,
- the page contents 

### Directory structure:
The TSE Crawler comprises of the following components: 

```
The full tree looks like this:

├── .gitignore
├── Makefile
├── README.md
├── common			
│   ├── .gitignore
│   ├── Makefile
│   ├── README.md
│   ├── pagedir.c
│   └── pagedir.h
├── crawler			
│   ├── .gitignore
│   ├── IMPLEMENTATION.md
│   ├── Makefile
│   ├── README.md
│   ├── testing.sh
│   ├── testing.out
│   └── crawler.c
├── libcs50 - library provided to us 
│   ├── .gitignore
│   ├── Makefile
│   ├── README.md
│   ├── bag.c 
│   ├── bag.h
│   ├── counters.c	
│   ├── counters.h
│   ├── file.c
│   ├── file.h
│   ├── file.md
│   ├── hashtable.c	
│   ├── hashtable.h
│   ├── jhash.c
│   ├── jhash.h
│   ├── libcs50-given.a
│   ├── memory.c
│   ├── memory.h
│   ├── memory.md
│   ├── set.c		
│   ├── set.h
│   ├── webpage.c
│   ├── webpage.h
│   └── webpage.md
```
My crawler consists of just one .c file. They make use of a common code, which I keep in the 'common' directory:

pagedir - a suite of functions to help the crawler write pages to the pageDirectory and help the indexer read them back in


### Major data structures/ modules used in the implementation of crawler: 

**Hashtable**

A hashtable is a set of (key,item) pairs. It acts just like a set, but is far more efficient for large collections. It contains the following functions, implemented in hashtable.c of the libcs50 directory:

```c
/**************** global types ****************/
typedef struct hashtable hashtable_t;  // opaque to users of the module

/**************** hashtable_new ****************/
hashtable_t *hashtable_new(const int num_slots);

bool hashtable_insert(hashtable_t *ht, const char *key, void *item);

void *hashtable_find(hashtable_t *ht, const char *key);

void hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item));

void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) );

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) );
```
Refer to hashtable.h for more details on the implementation of the above functions.

**Set (indirectly used by hashtable)**

A set maintains an unordered collection of (key,item) pairs; any given key can only occur in the set once. It starts out empty and grows as the caller inserts new (key,item) pairs. It contains the following functions, implemented in set.c of the libcs50 directory:

```c
/**************** global types ****************/
typedef struct set set_t;  // opaque to users of the module

/**************** functions ****************/
set_t *set_new(void);

bool set_insert(set_t *set, const char *key, void *item);

void *set_find(set_t *set, const char *key);

void set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item) );

void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) );

void set_delete(set_t *set, void (*itemdelete)(void *item) );
```
Refer to set.h for more details on the implementation of the above functions.


**Bag**

A bag is an unordered collection of items. The bag starts empty, grows as the caller adds one item at a time, and shrinks as the caller extracts one item at a time. It could be empty, or could contain hundreds of items. Items are indistinguishable, so the extract function is free to return any item from the bag. It contains the following functions, implemented in bag.c of the libcs50 directory:

```c
/**************** global types ****************/
typedef struct bag bag_t;  // opaque to users of the module

/**************** functions ****************/
bag_t *bag_new(void);

void bag_insert(bag_t *bag, void *item);

void *bag_extract(bag_t *bag);

void bag_print(bag_t *bag, FILE *fp, 
               void (*itemprint)(FILE *fp, void *item));

void bag_iterate(bag_t *bag, void *arg,
                 void (*itemfunc)(void *arg, void *item) );

void bag_delete(bag_t *bag, void (*itemdelete)(void *item) );
```
Refer to bag.h for more details on the implementation of the above functions.

### Other modules used / created: 

**webpage**

This module contains utility functions for downloading, saving, and loading web pages. 

```c
/***********************************************************************/
/* webpage_t: opaque struct to represent a web page, and its contents.
 */
typedef struct webpage webpage_t;

/* getter methods */
int   webpage_getDepth(const webpage_t *page);
char *webpage_getURL(const webpage_t *page);
char *webpage_getHTML(const webpage_t *page);

webpage_t *webpage_new(char *url, const int depth, char *html);

void webpage_delete(void *data);

bool webpage_fetch(webpage_t *page);

char *webpage_getNextWord(webpage_t *page, int *pos);

char *webpage_getNextURL(webpage_t *page, int *pos);

bool NormalizeURL(char *url);

bool IsInternalURL(char *url);
```
Refer to webpage.h for more details on the implementation of the above functions

**pagedir**

 This is a module containing utility functions for fetching, saving, and scanning web pages + integer to string converter.

```c
/**************** functions ****************/

/**************** pagefetcher ****************/
/* A page fetcher function that retrieve HTML from page->url, store into page->html
 *
 * Returns a boolean to keep track of success: true if the fetch was successful - false otherwise 
 * If the fetch succeeded, page->html will contain the content retrieved. 
 * Note that the page that caller provides must be allocated.
 * 
 * Since a buffer will be allocated in the webpage_fetch function, the caller must later free this memory, by calling webpage_delete().
 *
 */
bool pagefetcher(webpage_t *page);

/**************** pagesaver ****************/
/*
* A page saver function that writes the caller provided webpage to the pageDirectory with a unique ID. 
*
* This function is of type int, meaning it returns 0 if successful, non-negative integer otherwise. 
* 
* Takes in a webpage, a string path and an integer id as a parameter
* Creates a new file inside the pageDirectory, which is named the unique ID the caller provides. 
* Inside the file, the caller-provided webpage's URL, Depth and HTML are printed 
*
* Makes use of the getter functions provided in webpage.h 
*/
int pagesaver(webpage_t *page, char * path, int id);

/**************** pagescanner ****************/
/*
* A page scanner function that parses the caller-provided webpage to extract all its embedded URLs.
*
* This function is of type int, meaning it returns 0 if successful, non-negative integer otherwise. 
* 
* Takes in a webpage, an integer depth, a hashtable struct and a bag struct as parameters. 
* Loops through every extracted URL, using the webpage_getNextURL function from webpage.h (which returns the next url from html[pos])
* 	normalizes the URL 
*	if that URL is not ‘internal’, ignore it;
*	try to insert that URL into the hashtable of URLs seen
*		if it was already in the hashtable, do nothing;
*		if it was added to the hashtable,
*			make a new webpage for that URL, at depth + 1
*			add the new webpage to the bag of webpages to be crawled
*
*/
int pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag);

/**************** itoa ****************/
/*
* An integer to string converter 
* 
* Takes in an integer as a parameter, and converts it to a string (irrespective of the base) of base 10 
* Returns a char * string 
*
*/
char* itoa(int integer);
```
For further detail on how pagedir.c was implemented, look into pagedir.h.


### Functional decomposition into modules: 

main - parses parameters and passes them to the crawler.

crawler - uses a bag to track pages to explore, and hashtable to track pages seen; when it explores a page it gives the page URL to the pagefetcher, then the result to pagesaver, then to the pagescanner.

pagefetcher - fetches the contents (HTML) for a page from a URL and returns.

pagesaver - outputs a page to the appropriate file.

pagescanner - extracts URLs from a page and returns one at a time.


### Pseudocode:

A breakdown of the functions with pseudocode is provided here:

```c
// main function that takes in parameters argc, and a string array argv[] for us to later track what the command line has provided us

int main (int argc, char* argv[]);

int DirExistence (char *directory);
```

**Pseudocode for main:**

1. The main function takes in two parameters, an integer argc to count the number of arguments provided, and a string array argv[] for later use of indexing arguments

2. Then, the function checks whether the correct number of arguments (i.e. 4) is provided, and if not, it prints error message and exits with nonzero status

3. Next, the function validates the parameters: 
	- checks whether seedURL is valid and internal to our domain
	- checks whether the page directory exists and is writable (using the DirExistence() function)
	- checks if the crawl depth is between 0 and 5 (for testing purposes since crawling a large depth can take a while)

4.  Once all the above conditions pass through successfully, the main function will call the crawler function, using argv[1] as the seedURL, argv[2] as the page directory and atoi(arg[3]) as the depth

5. If everything is successful, returns 0 status.


```c
// crawler function that takes in a string URL, a directory name and an integer which delineates the maximum number of hops from the seedURL

int crawler(char* seedURL, char *dirName, const int maxDepth);
```

**Pseudocode for crawler:**

1. The crawler function takes in 3 parameters, a string seedURL, a string directory name and an integer depth

2. Creates a bag that holds URLs that need to be crawled 

3. Creates a hashtable that will be used to keep track of what pages we have visited 

4. Create a webpage for the seedURL, marked with depth = 0

5. Add that page to the bag of webpages to crawl

6. Add that URL to the hashtable of URLs seen

7. While there are more webpages to crawl (checked by using bag_extract function to attempt to extract pages from the bag of URLs to be crawled):

	a. extract a webpage (URL,depth) item from the bag of webpages to be crawled 

	b. use pagefetcher to retrieve a webpage for that URL.

	c. increment the number of pages visited by 1 (this will be used as the ID number for each page we save)

	d. use pagesaver to write the webpage to the directory with the unique ID that is assigned to the number of pages visited

	e. if the webpage's depth (found by using the getter function for webpage depth) is < maxDepth provided by user, explore the webpage to find links:

		i. use pagescanner to parse the webpage to extract all its embedded URLs

		ii. for every URL we extract, we want to normalise the URL, check that it is internal (if it is not, ignore it)

		iii. try to insert that URL into the hashtable of URLs seen

			-> if it was already in the table, do nothing;
			-> if it was added to the table, then:
				- make a new webpage for that URL, at depth + 1
				- add the new webpage to the bag of webpages that need to be crawled

8. If everything runs successfully, returns 0 status; otherwise, returns nonzero status 

```c
// pagesaver takes in a webpage structure, a string path, and an ID which corresponds to the name of the file that will be written into pagedirectory
int pagesaver(webpage_t *page, char * path, int numvisited);
```

**Pseudocode for pagesaver:**

1. This function takes in a webpage struct, a string path and an integer id 

2. It converts the id from integer to string using an itoa() function

3. using 'strcpy' and 'strcat' function to append and make a character array  that represents the path of the file in which we will save to

4. Using file open function, fopen(), we will open the file using the filepath we just created in 3., and print the following:
	- the page URL on the first line,
	- the depth of the page (where the seed is depth 0) on the second line,
	- the page HTML, beginning on the third line.

5. If it fails to print, an error message is printed and a non zero status is returned

6. Returns 0 status if all things run successfully, else, non-zero status

```c
// pagefetcher function which takes in a page, and calls the webpage_fetch function of webpage.h
bool pagefetcher(webpage_t *page);
```

**Pseudocode for pagefetcher:**

1. This function takes in a webpage struct, and runs the webpage_fetch function on the provided struct (refer to webpage.h for details on the webpage_fetch function)

2. it returns a boolean of true if it successfully fetches, else returns false.


```c
// pagescanner takes in parameters: webpage, a page depth, a hashtable and a bag
int pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag);
```

**Pseudocode for pagescanner:**

1. Takes in a webpage struct, an integer depth, a hashtable and a bag as parameters

2. The function loops through every extracted URL by calling webpage_getNextURL() on the webpage struct provided 

3. Checks if the URL that is returned from webpage_getNextURL() is normalised

4. Checks if the URL is also internal to our domain

5. If so, it will try to insert the URL into the hashtable of URLS seen

6. Next, it makes a new webpage for that URL and inserts it into the bag provided by the caller 

7. It returns a boolean of true if it successfully fetches, else returns false.


```c
// Extra function: used in pagesaver function to create the file path 
char* itoa(int integer);
```

**Pseudocode for itoa:**

1. Takes in an integer, which will be converted to string

2. Makes a buffer of size 32

3. Looks through the integer that was provided and divides it by 10 to be compared to the buffer --> which will convert it to the corresponding integer

4. returns the string


### Error Handling: 

In crawler.c:

- if incorrect usage, the exit status is 1 
- if the URL is not verifiable to be internal to our domain, the exit status is 2
- if an invalid page directory is given, the exit status is 3
- if a directory does not have write permission, exit status is 4
- if max depth is larger than allowed, exit status is 5 
- if successfully runs, returns 0 status 




