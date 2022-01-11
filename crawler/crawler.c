/*
*
* Tiny Search Engine Web Crawler - crawler.c
* 
* The TSE crawler is a  program that crawls the web and retrieves webpages starting from a seed URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.
*
* Name: Luca Lit 
* Date: 3 Feb 2020
*
* Usage: ./crawler <seedURL> <pageDirectory> <maxDepth>
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "memory.h"
#include "../libcs50/bag.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"


// Declaring function prototypes 
int crawler(char* seedURL, char *dirName, const int maxDepth);
//int pagesaver(webpage_t *page, char * path, int numvisited);
//bool pagefetcher(webpage_t *page);
//int pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag);
//char* itoa(int integer);
int DirExistence (char *directory);



// main function
int main (int argc, char* argv[]){

	// for testing, limiting max depth to be no more than 5
	const int MAXDEPTH = 5; 

	// check that the right number of arguments are supplied

	if(argc != 4) {
		fprintf(stderr, "Incorrect usage. Please make sure to supply 4 arguments: ./crawler <seedURL> <pageDirectory> <maxDepth>\n");
		exit(1); 
	}

	// validate parameters
	// check that the seedURL is valid
	char *seedURL = argv[1];
	// allocate memory space to store the first argument (the seedURL)
	char *seedURLcopy = calloc(strlen(seedURL) + 1, sizeof(char));
	strcpy(seedURLcopy, seedURL);

	// check that the seedURL is valid and internal to our domain
	if (!IsInternalURL(seedURLcopy)) {
		fprintf(stderr, "Could not verify URL to be internal to our domain.\n");
		exit(2);
	}

	// check that pageDirectory exist and is writable
	char *pagedir = argv[2];

	if (DirExistence(pagedir) != 0){
		fprintf(stderr, "Invalid page directory. Check that it exists and is writable.\n");
		exit(3);
	}
	/*
	// making use of the unistd library to check if the directory exists 
	if (access(pagedir, F_OK) != 0){
		fprintf(stderr, "Invalid page directory. Make sure the directory exists.\n");
		exit(3);	
	}

	// checking if the directory has write access permission
	if (access(pagedir, W_OK) != 0){
		fprintf(stderr, "Provided directory does not have write permission.\n");
		exit(4); 
	}
	*/

	// check that the user provided maxdepth is less than what we allow (for testing), and that it is >= 0
	int maxdepth = atoi(argv[3]);
	if (maxdepth < 0 || maxdepth > MAXDEPTH) {
		fprintf(stderr, "Crawl depth must be 0 <= x <= %d, where x is an integer. \n", MAXDEPTH);
		exit(5);
	}

	// calling the crawler function with the parameters being: seedURL, page directory, and maxdepth
	crawler(seedURLcopy, pagedir, maxdepth);
	//free(seedURLcopy);
	return 0;
}



 // crawler function, which is of type void 
int crawler(char* seedURL, char *dirName, int maxDepth) {
	// creating an integer variable to keep track of number of pages visited. 
	int num_visited = 0;

	// create bag for URLS to crawl
	bag_t *URLbag = bag_new();
	if (URLbag == NULL){
		fprintf(stderr, "Unsuccessful in creating bag to store URLS to crawl.\n");
		return 4; 
	}

	
	// create the hashtable - if it fails, return non negative number
	hashtable_t *hashtable = hashtable_new(200);	
	if (hashtable == NULL) {
		fprintf(stderr, "Error creating hashtable\n");
		return 5; 
	} 


	// create a new webpage struct by calling the webpage_new function in webpage.h
	webpage_t *seedwebpage = webpage_new(seedURL, 0, NULL); 
		/*if (!NormalizeURL(seedURL)){
			return 6;
		}*/

	// if the webpage failed to create, then exit with non zero status
	if (seedwebpage == NULL) exit(6);

	// add this seed page to the bag 
	bag_insert(URLbag, seedwebpage);

	// add the webpage's URL to the hashtable, which keeps track of the URLs that are seen 
	hashtable_insert(hashtable, webpage_getURL(seedwebpage), "");


	webpage_t *currpage;

	// loop through every URl in the bag 
	while ((currpage = bag_extract(URLbag)) != NULL){

		// increment the number of visited URLs by 1
		num_visited ++;

		// call the page fetcher function to retrieve HTML from page-> url and store it into page->html 
		if (!pagefetcher(currpage)){
			printf("Failed to read page\n");
				continue;
		}

		printf("New page fetched!\n");
		// call the page saver function from pagedir.h, which writes the webpage to the pageDirectory with a unique ID (which we ascribe as the num_visited).
		//pagesaver(currpage, dirName, num_visited);
		if (pagesaver(currpage, dirName, num_visited) == 0) {
			printf("Page saved!\n");
		}


		// check that the depth of the current webpage is less than the max depth the caller provides 
		if (webpage_getDepth(currpage) < maxDepth) {
			// call page scanner function which parses the current webpage to extract all its embedded URLs;
			//pagescanner(currpage, webpage_getDepth(currpage), hashtable, URLbag);
			printf("Scanning pages now...\n");
			pagescanner(currpage, webpage_getDepth(currpage), hashtable, URLbag);
		}

		// delete the webpage - this also frees the pointers
		webpage_delete(currpage);

		}

	// delete the hashtable, a function of libcs50
	hashtable_delete(hashtable, NULL);

	// delete the bag, a function of libcs50
	bag_delete(URLbag, NULL);
	printf("Crawl complete.\n");
	
	return 0;
}



int DirExistence (char *directory){

      char* path = malloc(strlen(directory) + strlen("/.crawler")*sizeof(char));
      strcpy(path,directory);
      strcat(path, "/.crawler");
        
      FILE* fp = fopen(path,"w");

      if(fp == NULL){	
		return 1;
      }
      
      fclose(fp);
      free(path);
      return 0;
  }



/*
bool pagefetcher(webpage_t *page){
	return webpage_fetch(page);
}

void pagesaver(webpage_t *page, char * path, int id){
	char* IDstring = itoa(id);
	printf("%s\n", IDstring);
	char direcpath [strlen(path)+strlen(IDstring)+2];
	strcpy(direcpath, path);

	strcat(direcpath, "/");
	strcat(direcpath, IDstring);
	FILE* fp = fopen(direcpath, "w");

	if (fp != NULL) {
		fprintf(fp, "%s\n", webpage_getURL(page));
		fprintf(fp, "%d\n", webpage_getDepth(page));
		fprintf(fp, "%s\n", webpage_getHTML(page));
	}
	else {
		fprintf(stderr, "Failed to write to file\n");
	}
	fclose(fp);
}


void pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag){
	int position;
	position = 0; 
	char* blank="";

	char * res = NULL;

	while ((res = webpage_getNextURL(webpage, &position)) !=  NULL){
		char * resultURL = calloc(strlen(res)+1, sizeof(char));
		strcpy(resultURL, res);

		if (NormalizeURL(resultURL)){
			if (IsInternalURL(resultURL)){
				if (hashtable_insert(hashtable, resultURL, blank)){
					webpage_t *newwebpage = webpage_new(resultURL, currpageDepth, NULL);

					if (newwebpage == NULL){
						fprintf(stderr, "Failed to make new web page\n");
						webpage_delete(newwebpage);
						continue;
					}

					bag_insert(bag, newwebpage);
				}
				else {
					free(resultURL);
				}

			}
			else {
				free(resultURL);
			}
		}
		else {
				free(resultURL);
			}

		free(res);
		fprintf(stdout, "Found the embedded URLs!\n");
	}

}



char* itoa(int integer){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; integer && i ; --i, integer /= 10)
	
		buf[i] = "0123456789abcdef"[integer % 10];
	
	return &buf[i+1];
	
}
*/
