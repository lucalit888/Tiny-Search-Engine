# CS50 Lab 4 - Tiny Search Engine Crawler
## Luca Lit, CS50 Winter 2020

### Crawler:

The TSE Crawler crawls a website and retrieves webpages starting with a specified URL, called the seedURL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limits itself to maxDepth "hops" from the seed URL and to URLs that are ‘internal’ to the designated CS50 server.

### Usage

The crawler module is implemented by crawler.c, which contains the following functions: 

/**********************************functions**********************************/

```c

int main (int argc, char* argv[]); 

int crawler(char* seedURL, char *dirName, const int maxDepth);

int DirExistence (char *directory);

// From pagedir.h

int pagesaver(webpage_t *page, char * path, int numvisited);

bool pagefetcher(webpage_t *page);

int pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag);

char* itoa(int integer);

```

For more information on pagesaver, pagefetcher and pagescanner, refer to pagedir.h. 

### Implementation

The crawler will run as follows:

The main function will check whether the arguments provided are the correct number and the correct order, as per the following usage: 
./crawler seedURL pageDirectory maxDepth

where seedURL is used as the initial URL (must be an internal URL),
where pageDirectory is the pathname for an existing directory in which to write downloaded webpages, and
where maxDepth is a non-negative integer representing the maximum crawl depth (a maximal allowable value for maxDepth can be set in the code)

After validating all the parameters and the program will call the crawler() function, which implements the following: 

It will make a webpage for the seedURL, marked with depth=0, and add that page to the bag of webpages to crawl. 
In additioin, it will add that seedURL to the hashtable of URLs seen
Next, while there are more webpages to crawl, the program will:
 => extract a webpage (URL,depth) item from the bag of webpages to be crawled
 => pause for at least one second
 => use pagefetcher() to retrieve a webpage for that URL,
 => use pagesaver() to write the webpage to the pageDirectory with a unique ID (ascribed by the number of pages visited), 
 => if the webpage's depth is less than the maxDepth specified by the caller, explore the webpage to find links:
 	=> use pagescanner to parse the webpage to extract all its embedded URLs;
	=> for each extracted URL:
		=> check if the URL is normalised by calling NormalizeURL from webpage.h
		=> if that URL is not ‘internal’, ignore it and do nothing
		=> if the above conditions are satisfied, try to insert that URL into the hashtable of URLs seen
			=> if it was already in the table, do nothing;
			=> if it was added to the table,
				=> make a new webpage for that URL, at depth+1
				=>add the new webpage to the bag of webpages to be crawled


### Compilation

To compile, simply `make`.

To test, do `make test`.

To clean, do `make clean`.

