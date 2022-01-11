/* 
 * pagedir.h - header file 
 * 
 * Containing utility functions for fetching, saving, and scanning web pages + integer to string converter + checking whether a directory was produced by the crawler
 *
 * Luca Lit - Feb 7 2020
 *
 */


#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "hashtable.h"
#include "bag.h"

/**************** functions ****************/

/**************** isCrawlerDirectory ****************/
/* A function that checks whether the directory provided by the  caller is a directory produced by the crawler.
*
* Takes in a string representing the directory name as a parameter. 
* Since it is of type int, it returns 0 if it successfully read the file, 1 if not 
*
*/

int isCrawlerDirectory(char *dir);

/**************** pagefetcher ****************/
/* A page fetcher function that retrieve HTML from page->url, store into page->html
 *
 * Returns a boolean to keep track of success: true if the fetch was successful - false otherwise 
 * If the fetch succeeded, page->html will contain the content retrieved. 
 * Note that the page that caller provides must be allocated.
 * 
 * since a buffer will be allocated in the webpage_fetch function, the caller must later free this memory, by calling webpage_delete().
 *
 * Assumptions:
 *     1. page has been allocated by caller
 *     2. page->url contains the url to fetch
 *     3. page->html is NULL at call time
 *
 * Limitations:
 *   * can only handle http (not https or other schemes)
 *   * can only handle URLs of form http://host[:port][/pathname]
 *   * cannot handle redirects (HTTP 301 or 302 response codes)
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

#endif

