/*
* CS50 Lab 5 - indexer.c
* 
* Name: Luca Lit 
* Date: Feb 16 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include "hashtable.h"
#include "webpage.h"
#include "counters.h"
#include "file.h"
#include "index.h"
#include "word.h"
#include "pagedir.h"

// function prototypes
indx_t* buildinvertedindex(char* pageDirectory, indx_t* indx);
static void KIdelete(void* i);

int main(int argc, char *argv[]){

	// Validate parameters 
	// check that the correct number of arguments is entered 
	
	if (argc != 3) {
		fprintf(stderr, "Incorrect number of arguments inputted. Usage: ./indexer pageDirectory indexFilename.\n");
		exit(1);
	}

	if (access(argv[1], F_OK) != 0) {
		fprintf(stderr, "Invalid directory. Please provide valid one.\n");
		exit(2);
	}
	// check whether directory entered was created by the crawler 
	if (isCrawlerDirectory(argv[1]) == 1) {

		fprintf(stderr, "Directory not produced by Crawler\n");
		exit(3);
	}
	// check filename (third arg) is valid (i.e. exists and can be read)
	// making use of the unistd library to check if the file exists 

	/*if (access(argv[2], F_OK) != 0){
		fprintf(stderr, "File does not exist\n");
		exit(4);	
	}

	// checking if the file has write access permission
	if (access(argv[2], W_OK) != 0){
		fprintf(stderr, "File does not have write permission\n");
		exit(5); 
	}
*/

	indx_t* indx = indx_new(700);
	indx = buildinvertedindex(argv[1], indx);

	index_saver(argv[2], indx);
	hashtable_delete(indx, KIdelete);

	return 0;

}


indx_t * buildinvertedindex(char* pageDirectory, indx_t* indx){

	struct dirent *dp;
	DIR *pageDir = opendir(pageDirectory);
	int id = 1;

	char file[700];
 	// while there are still files in the directory to be read 
	while ((dp = readdir(pageDir)) != NULL) {

		// allocate memory space for the file buffer
		/*//char * file = malloc(*sizeof(char) strlen(pageDirectory) + strlen("/%d"));
		char* file = calloc(strlen(pageDirectory) + strlen("/%d"),sizeof(char));
		// write to "file" in the form of pagedirectory/id
		char * stringID = itoa(id);
		strcpy(file, pageDirectory);
		strcat(file, "/");
		strcat(file, stringID);
		printf("%s\n", file);
		// check if file and be read
		*/
		
		sprintf(file, "%s/%s", pageDirectory, dp->d_name);
		printf("Opening file '%s'\n", file);

		FILE * fp = fopen(file, "r");

		if (fp == NULL){
			fprintf(stderr, "Could not read file\n");
			continue; 
		}

		// open the file
		//fprintf(stdout, "Opening file '%s' ... \n", file);


		// Create a data structure of webpage using the opened file
		// define strings (first line of file is URL, second is DEPTH, third is the HTML)
		char *fileURL;
		char *fileDEPTH;
		char *fileHTML;
		webpage_t * webpage;
		int fileDEPTH_int;

		// check that none of the URL, Depth nor HTML are NULL
		//if(((fileURL = freadlinep(fp)) != NULL) && ((fileDEPTH = freadlinep(fp)) != NULL) && ((fileHTML = freadfilep(fp)) != NULL)) {

		if ((fileURL = freadlinep(fp)) == NULL) {
			printf("Failed to read the URL\n");
			printf("Continuing to the next file ... \n");
			printf("\n");
			fclose(fp);
			continue; 	
		}
	
		if ((fileDEPTH = freadlinep(fp)) == NULL){
			printf("Failed to read the DEPTH\n");
			printf("Continuing to the next file ... \n");
			printf("\n");
			free(fileURL);
			fclose(fp);
			continue; 
		}

		if ((fileHTML = freadfilep(fp))== NULL){
			printf("Failed to read the HTML\n");
			printf("Continuing to the next file ... \n");
			printf("\n");
			free(fileURL);
			free(fileDEPTH);
			fclose(fp);
			continue; 
		}
		
			
		printf("URL: %s \n", fileURL);
		printf("Depth: %s \n", fileDEPTH);
		printf("HTML: %s \n", fileHTML);
	
		// convert the file depth from a string to an integer and store it in another variable 
		fileDEPTH_int = atoi(fileDEPTH);
		//printf("testing\n");
		// create a new webpage struct, in which, we insert the file's URL, DEPTH (in int form) and the HTML as parameters
		webpage = webpage_new(fileURL, fileDEPTH_int, fileHTML);
			
		//} 
		// if they were NULL, just continue to the next iteration
		/*else{
			printf("Continuing to next iteration ... \n");
			printf("\n");
			fclose(fp);
			continue;

		}*/
		
		// Reading and extracting words from the HTML / inserting to index hashtable
		char * res; 
		char * normres;
		int position = 0;
		counters_t* counts;

		// there is a next word to extract 
		while ((res = webpage_getNextWord(webpage, &position)) != NULL) {
			

			// check whether it is at least 3 characters long
			if(strlen(res) > 2){
				normres = NormalizeWord(res);
				// if indx_find cannot find that word
				if ((counts = indx_find(indx, normres)) != NULL) {
					int dpINT = atoi(dp->d_name);
					counters_add(counts,dpINT);
				}

				// if it was unable to find, create a new counterset 
		    	else{
		    		counts = counters_new();
		    		// convert to integer, so that we can use counters_add
		    		int dpINT = atoi(dp->d_name);
		    		counters_add(counts,dpINT);
		    		// insert into the indx hashtable the normalised word, and the count (how many times it appeared)
		    	}
		    	indx_insert(indx,normres,counts);
		   		//counters_add(counts,id);
		    }

		    
		    free(res);
		    
		}

		id++;
		fclose(fp);

		free(fileDEPTH);

		//if (pageDirectory != NULL){
		webpage_delete(webpage);

	}
	free(dp);
	free(pageDir);

	return indx;

}

// A function for Key item delete
static void KIdelete(void *i){

	counters_t * ct = i;
  	if (ct != NULL) {
    	counters_delete(ct);
  	}

}

// A function that normalises a word, meaning, turns it into lowercase 
/*
char * NormalizeWord(char * string){
  unsigned char *S = (unsigned char *) string;

  while (*S) {
  	// making use of tolower() function in ctype.h
     *S = tolower((unsigned char)*S);
      S++;
  }
  return string;
}
*/

/*void index_saver(hashtable_t* indx, char* nameoffile){
	
	FILE* fp = fopen(nameoffile, "w");
	hashtable_iterate(indx,fp,item_saver);
}


void item_saver(void *arg, const char *key, void *item){
	
	FILE* fp = arg;
	counters_t *count = item;

	fprintf(fp, "%s", key);

	counters_iterate(count,fp,count_save);
	fprintf(fp,"\n");
}

void count_saver(void *arg, const int key, int count){
	FILE* fp = arg;

	fprintf(fp, " %d : %d", key, count);
}
*/

