/*
* index.c - utility functions for creating, finding and inserting into new indexes (hashtable structs) as well as loading and saving functions 
* Refer to index.h for more information
*
* Name: Luca Lit
* Date: Feb 15 2020
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include "hashtable.h"
#include "webpage.h"
#include "counters.h"
#include "file.h"
#include "index.h"
#include "word.h"
#include "pagedir.h"

// Define a new struct called indx_t
typedef hashtable_t indx_t;

// Functino to create a new index struct
indx_t *indx_new(const int num_of_slots){

	return hashtable_new(num_of_slots);
}

// Function that inserts a counterset, given an index and a key
// since hashtable insert returns bool, our func type is also bool
bool indx_insert(indx_t *indx, char *key, counters_t *counts){

	return hashtable_insert(indx, key, counts);
}

// Function that takesin an index struct and a key, and returns the counterset that hashtable_find() finds
counters_t* indx_find(indx_t* indx, char* key){
	
	return hashtable_find(indx, key);
	
}


// a function that loads an index file produced by the indexer
//Input Parameter is a filename and an empty index file - and returns a hashtable, (or index), 

indx_t* index_loader(char * nameoffile, indx_t * indx) {

	char* newword;
	int id;
	int count;


	// first open the file, for read
	FILE * fp = fopen(nameoffile, "r"); 

	// check that it was indeed able to open
	if (fp == NULL) {
		fprintf(stderr, "File could not be opened / read. \n");

		return NULL;
	}	
	// if file successfully opens and is readable, 
	// use freadwordp() function from file.h in libcs50
	while((newword = freadwordp(fp))!= NULL){
		// initialise new counter
		counters_t* newcounter = counters_new();
		// insert into hashtable using the user provided index file, followed bny id, and a counter struct
		hashtable_insert(indx, newword, newcounter);

		while(fscanf(fp, "%d %d", &id, &count) == 2){
			counters_set(newcounter, id, count);
		}

		// remember to free memory that was allocated through freadwordp
		free(newword);
	}

	// close the file 
	fclose(fp);
	return indx;
}


// a function that saves the index that calls item_saver and count_saver (indirectly)
void index_saver(char* nameoffile, hashtable_t* indx){
	
	// first open the file 
	FILE* fp = fopen(nameoffile, "w");

	// use hashtable iterate to call key saver function
	hashtable_iterate(indx,fp,key_saver);
	fclose(fp);
}

// a function that calls counters_iterate and applies count_save function / also prints word to the file
void key_saver(void *arg, const char *key, void *i){

	// set the arg to  the file to be written into (the same  one as we opened in index saver)
	FILE* fp = arg;
	// set the item parameter
	counters_t *c = i;

	fprintf(fp, "%s", key);
	// calls counters_iterate function from counters.h and applies count_saver 
	counters_iterate(c,fp,count_saver);

	fprintf(fp,"\n");

}

// a function that print item to the file fp in the form (key : count)

void count_saver(void *arg, const int key, int count){
	// takes the first parameter and sets it to the file

	FILE *fp = arg; 
	fprintf(fp, " %d %d", key, count);
}


// a function that load the index from the oldIndexFilename into an inverted-index data structure.





