/*
*
* Tiny Search Engine Web Crawler - pagedir.c
*
* Name: Luca Lit 
* Date: 3 Feb 2020
*
* Usage: contains functions that are called in crawler.c - for fetching, saving, and scanning web pages + integer to string converter
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#include "webpage.h"
#include "bag.h"
#include "memory.h"
#include "bag.h"
#include "webpage.h"
#include "hashtable.h"
#include "pagedir.h"


// function that checks whether the directory provided by the user was produced by the crawler
int isCrawlerDirectory(char *dir){

	// create a path that is the string length of user provided dir
	char filepath[700];
	sprintf(filepath, "%s/.crawler", dir);

	if(access(filepath, R_OK) != -1){
		return 0;
	}
		return 1;
	
	/*char * filepath = calloc(strlen(dir) + strlen("/.crawler"), sizeof(char));
	strcpy(filepath, dir);
	strcat(filepath,"/.crawler");*/
	//FILE * fp = fopen(filepath, "r");
	// check if the file is NULL, if so return nonzero status

	/*if (fp == NULL){
		free(filepath);
		return 1;
	}
	free(filepath);
	fclose(fp);
	return 0;*/
	// checks whether the directory that follows this path is readable
}

// pagefetcher function which simply calls webpage_fetch from webpage.h, and returns a boolean to tell if fetch was successful
bool pagefetcher(webpage_t *page){
	return webpage_fetch(page);
}


// page saver function which takes in a page, a path and an unique ID as parameters. 
int pagesaver(webpage_t *page, char * path, int id){

	// use the itoa function to convert the caller provided (int id) into a string
	char* IDstring = itoa(id);

	// check by printing the converted string 
	printf("Number of pages visited: %s\n", IDstring);

	char direcpath [strlen(path)+strlen(IDstring)+2];

	// copy the caller prov ided path to the char directpath
	strcpy(direcpath, path);

	// concatenate a / to the end of direcpath
	strcat(direcpath, "/");
	// concatenate the ID number of the page at the end of direcpath - the result is something like e.g. data/1
	strcat(direcpath, IDstring);

	// open the file that has the path of direcpath, and write into it 
	FILE* fp = fopen(direcpath, "w");

	// if the file is valid 
	if (fp != NULL) {
		// print the URL of the page
		fprintf(fp, "%s\n", webpage_getURL(page));
		// print the depth of the page 
		fprintf(fp, "%d\n", webpage_getDepth(page));
		// print the HTML of the page 
		fprintf(fp, "%s\n", webpage_getHTML(page));
	}
	else {
		// if there was an error, print to standard error that there was an error writing to the file 
		fprintf(stderr, "Failed to write to file\n");
		return 8;
	}
	// make sure to close the file when done
	fclose(fp);
	return 0; 
}



// a pagescanner function of type int which parses the caller-provided webpage to extract all its embedded URLs;
int pagescanner(webpage_t* webpage, int currpageDepth, hashtable_t *hashtable, bag_t *bag){
	int position;
	position = 0; 
	char* blank="";

	char * res = NULL;

	// looping through every extracted URL
	while ((res = webpage_getNextURL(webpage, &position)) !=  NULL){
		// allocate memory space to store the extracted URL
		char * resultURL = calloc(strlen(res)+1, sizeof(char));
		// copy res into the resultURL space we just made 
		strcpy(resultURL, res);

		// check that the URL is normalised 
		if (NormalizeURL(resultURL)){
			// check the URL is internal to our domain 
			if (IsInternalURL(resultURL)){
				printf("Embedded URL at depth %d is: %s\n", currpageDepth, resultURL);

				// try to insert that URL into the hashtable of URLs seen
				if (hashtable_insert(hashtable, resultURL, blank)){
					//if successfully inserted URL to the hashtable, then make a new webpage for that URL, at depth+1
					webpage_t *newwebpage = webpage_new(resultURL, currpageDepth+1, NULL);
					printf("New webpage inserted into hashtable.\n");

					// check that the webpage made successfully
					if (newwebpage == NULL){
						fprintf(stderr, "Failed to make new web page.\n");
						webpage_delete(newwebpage);
						continue;
					}

					// add the new webpage to the bag of webpages to be crawled
					bag_insert(bag, newwebpage);
					printf("New webpage inserted into bag.\n");
				}
				// if the hashtable failed to make 
				else {
					free(resultURL);
				}

			}
			// if the URL was not internal
			else {
				free(resultURL);
			}
		}
		// if the URL was not normalised
		else {
				free(resultURL);
			}

		free(res);
		//fprintf(stdout, "Found an embedded URL!\n");
	}

	return 0;
}



// a function called itoa - which converts integer to string - takes in the integer as the parameter - used in my pagesaver function to create the file path 
char* itoa(int integer){
	
	// make a buffer 
	static char buf[32] = {0};
	int i = 30;
	
	// loop through every integer and divide it by 10 (modular)
	for(; integer && i ; --i, integer /= 10)
		// convert it to a string 
		buf[i] = "0123456789abcdef"[integer % 10];

	return &buf[i+1];
	
}



