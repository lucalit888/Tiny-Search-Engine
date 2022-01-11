# CS50 Lab 4 - Crawler- DESIGN.md 
## Luca Lit - Feb 16 2020

### Design specifications

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