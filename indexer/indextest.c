/*
* indextest.c - tester for the indexer module
* 
* Usage: ./indextest oldIndexFilename newIndexFilename
*
* Name: Luca Lit
* Date: Feb 15 2020
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"


static void KIdelete(void *i);



int main (int argc, char *argv[]){

	char* oldIndexFilename;
	char* newIndexFilename;

	// Validate paramteres by verifying the correct number of parameters
	
	if(argc != 3){
		// print error message and exit with 1
		fprintf(stderr, "Incorrect number of arguments inputted. Usage: ./indextest oldIndexFilename newIndexFilename.\n");
		exit(1);
	}

	// if parameters are correct
	// create a new hashtable struct
	indx_t* indx = indx_new(700);

	// set the string oldIndexname to the first argument inputted by user
	oldIndexFilename = argv[1];
	// call load_index from index.h 
	// this loads the index from oldIndexFilename into an inverted-index data structure.
	indx = index_loader(oldIndexFilename, indx);

	newIndexFilename = argv[2];
	// save the index using index_saver from index.h
	// creates a file newIndexFilename and writes the index to that file
	index_saver(newIndexFilename, indx);

	// remember to delete the hashtable when all processes are done
	hashtable_delete(indx, KIdelete);

	return 0;
}


static void KIdelete(void *i){

	counters_t * ct = i;
  	if (ct != NULL) {
    	counters_delete(ct);
  	}

}
