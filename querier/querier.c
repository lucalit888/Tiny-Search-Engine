/*
 * CS50 WINTER 2020 LAB 5 - querier.c
 *
 * Name: Luca Lit
 * Date: 19 Feb 2020
 *
 * Citing: geeks4geeks for the quicksort function, and help from my tutor.
 *
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


// Defining a structure that holds two counters (used for intersecting sets later)
typedef struct countersetpair_t {
  counters_t *ctrs_base;
  counters_t *ctrs_loop;
} countersetpair_t;

// Defining a structure which holds an ID and a score (used for ranking later)
typedef struct docID_score_t {
  int doc_ID;
  int rankscore;
} docID_score_t;

// Defining a structure which holds an array of doc_ID_score_t, and an integer which represents the index
typedef struct arrPos_t {
  docID_score_t ** arr;
  int pos;
} arrPos_t;


// function prototypes
//int readandrank(FILE * fp, indx_t* indx);
int readandrank(FILE * fp, indx_t* indx, char * pageDirectory);
int parse_clean(char * input, char * array[], int *actualarraysize);
static void counters_intersect(void * countersetpair_t, const int doc_ID, int count);
static void countercopy(void * arg, const  int doc_ID, int count);
static void union_(void * unionizedset, const int key, int count);
static void counting(void *t, const int doc_ID, int count);
void ranker(counters_t * ctrset, int num_matches, char * pageDirectory);
char * returnURL(char * pageDirec, int doc_ID);
static void sort_array(docID_score_t *dIs [], int lo, int hi);
static int partitioning(docID_score_t *dIs[], int lo, int hi);
static void ctrs_into_arr(void *arrPos, const int key, int c);
static void KIdelete(void *i);



// Main function to validate parameters + loads indexfile  

int main(int argc, char *argv[]){

	// Validate parameters, check that usage is correct: ./querier pageDirectory indexFilename
	// where pageDirectory is the pathname of a directory produced by the Crawler, and
	// where indexFilename is the pathname of a file produced by the Indexer.

	// check that the correct number of arguments is supplied 
	if (argc != 3) {
		fprintf(stderr, "Incorrect number of arguments inputted. Usage: ./querier pageDirectory indexFilename.\n");
		exit(1);
	}

	// check whether the directory exists
	char * pageDirectory = argv[1];
	FILE *fp1 = fopen(pageDirectory, "r");
	if (fp1 == NULL){
		fprintf(stderr, "pageDirectory does not exist or is not readable\n");
		exit(2);
	}
	fclose(fp1);

	// check whether directory entered was created by the crawler 
	if (isCrawlerDirectory(argv[1]) == 1) {
		fprintf(stderr, "%s is not produced by Crawler\n", argv[1]);
		exit(3);
	}

	// check whether the second argument, the file path for a file index, exists
	char * indexFilename = argv[2];
	FILE *fp = fopen(indexFilename, "r");
	if (fp == NULL){
		fprintf(stderr, "Indexfilename does not exist or is not readable\n");
		exit(4);
	}

	fclose(fp);

	// loads the index
	indx_t* indx;
	indx = indx_new(700);
	indx = index_loader(argv[2], indx);
	
	// calls read and rank
	readandrank(stdin, indx, argv[1]);
	
	// calls hashtable delete to free memory
	hashtable_delete(indx, KIdelete);
	return 0;

}


// Reads standard input + calls parse_clean() on stdin, and ranks the documents. 

int readandrank(FILE * fp, indx_t* indx, char * pageDirectory) {

	char * input;	

// for querying, parsing, cleaning and printing a 'clean' query
	printf("Query? ");
	//while ((input = freadlinep(stdin)) != NULL){
	while (!feof(fp)){

		input = freadlinep(stdin);

		// check if input is NULL, if so return and free
		if (input == NULL){
			printf("\n");
			free(input);
			return -1;
		}


		char * words[700];
		int actualarraysize = 0; 

		// parses and cleans the standard input 
		if(parse_clean(input, words, &actualarraysize) != 0){
			free(input);
			continue;
		}

		// if the query is actually more than one word
		if (actualarraysize > 0 ){
			printf("Query: ");

            for (int i = 0; i < actualarraysize; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");



            // reading the word array and finding their scores
		    counters_t *finalset[100];
		    int finalset_ind = 0;

	        counters_t * ctrset_temp;
	        counters_t * ctrset_temp2;
	        ctrset_temp = counters_new();

	        // Read the  first word of the array
	      	printf("Reading first word of the array: %s\n", words[0]);
	        if (indx_find(indx, words[0]) == NULL) {
	        	printf("First word does not exist in the index\n");
	        	ctrset_temp = counters_new();

	        }else {
	        	// if the word has a corresponding counterset, then make a copy
	        	counters_iterate(indx_find(indx, words[0]), ctrset_temp, countercopy);
	        }
	        
	        //ctrset_temp = indx_find(indx, words[0]);
	        // copying the counterset to ctrs_temp()
			//counters_print(ctrset_temp, stdout);
	       	// printf("\n");

	        int num_of_matches = 0;
	        //

	        // check whether the array size is larger than 1
	        if (actualarraysize > 1) {
	        	// if it is, loop through all the words in the array
	        	for (int i = 1; i < (actualarraysize); i++){

	        		printf("Reading next word ... \n");

	        		// if an and is encountered
	        		if (strcmp(words[i], "and") == 0){
	        			printf("encountered an 'and' - skipping over it \n");
	        			continue;
	        		}

	        		// if an or is encountered 
	        		if (strcmp(words[i], "or") != 0){
	        			// if the word is the end of the array
	        			if (words[i] == NULL) {
	        				printf("reached the end of the word array!\n");
	        				
	        				finalset[finalset_ind] = ctrset_temp;
	        				finalset_ind ++;
	        				

	        				break;
	        			}

	        			// if the word does not exist in the index
	        			if (indx_find(indx, words[i]) == NULL) {
	        				printf("Word does not exist in index \n");

	        				// if NULL, create new counterset
	        				if (i + 1 >= actualarraysize){
	        					if (indx_find(indx, words[i]) != NULL){
	        						finalset[finalset_ind] = ctrset_temp;
	        						
	        					}

	        				}

	        				continue;
	        			}

	        			// if the word is not or, and is not and then do the following
	        			counters_t * nextSet;
	        			nextSet = indx_find(indx, words[i]);
	        			//counters_print(nextSet, stdout);
	        			//printf("\n");

	        			countersetpair_t * ctrset_pair = (malloc(sizeof(countersetpair_t)));

	        			if (ctrset_pair == NULL){
	        				fprintf(stderr, "Failed to allocate memory for counterset pair\n");
	        				//counters_delete(nextSet);
	        				break;
	        			} 

	        			ctrset_pair->ctrs_base = ctrset_temp;
	        			ctrset_pair->ctrs_loop = nextSet;

						// Intersect the two sets using count iterate and the counters_intersect helper
	        			counters_iterate(ctrset_temp, ctrset_pair, counters_intersect);
	        			
	        			free(ctrset_pair);
	        			//counters_print(ctrset_temp, stdout);
	        			//printf("\n");

	        			if ((i + 1) == actualarraysize){	
	        				
	        				finalset[finalset_ind] = ctrset_temp;
	        				finalset_ind ++;
	        		
	        				//counters_print(finalset[finalset_ind], stdout);
	        			}

	        			//counters_delete(nextSet);

	        			
	        		} else {
	        			// if an or is encountered
	        			printf("encountered an 'or' - adding a new andsequence to the array\n");
	        		
	        			// save the temporary counterset to the array of final sets (to be unioned)
	        			finalset[finalset_ind] = ctrset_temp;

	        			// if the next word does not exist in index, then initialise new counter
						if (indx_find(indx, words[i+1]) == NULL) {
							ctrset_temp = counters_new();
							
						} else {
							//ctrset_temp = indx_find(indx, words[i+1]); 
							ctrset_temp2 = counters_new();

							counters_iterate(indx_find(indx, words[i + 1]), ctrset_temp2, countercopy);
							ctrset_temp = ctrset_temp2;							
						}

		        		//printf("counterstemp reset to: \n");
		        		//counters_print(ctrset_temp, stdout);
		        		//printf("\n");
		        		finalset_ind++; 

	        		}

	        	}

	        	//counters_print(finalset[finalset_ind], stdout);

	        	printf("All and sequences read and saved. Proceeding to handle 'or's now ...\n");

        
	        	counters_t *unionized = counters_new();

	        	//printf("%d\n", finalset_ind);
	        	// if the finalset_ind > 0, meaning there are at least 2 countersets in the array of sets to be unioned
	        	if (finalset_ind > 0) {
	        		// perform merging of two sets
		        	for (int j = 0; j < (finalset_ind); j++) {
		        		printf("Merging two sets \n");
		        		counters_iterate(finalset[j], unionized, union_);
		        		// free the counterset
	        			counters_delete(finalset[j]);
		        		printf("Successfully merged! \n");

		        		
		        	}
		        		// print the merged set
		        		printf("Merged Set: ");
	        			counters_print(unionized, stdout);
	        			printf("\n");
	        			// rank them and print to console
	        			ranker(unionized, num_of_matches, pageDirectory);

	        	} 
	        	else {
	        		// if the finalset's first element is NULL, it means that no documents matched the query. 
	        		if (finalset[0] == NULL){
	        			printf("No documents match the query. \n");
	        		}
	        		counters_delete(ctrset_temp);
	        		
	        		//counters_iterate(finalset[0], unionized, countercopy);
	        	}

        	// count the number of documents in the finalset
	        	//ranker(unionized, num_of_matches, pageDirectory);
	            counters_delete(unionized);
	              
   	
        	}

        	// if the array size was just one singular word 
        	else if (actualarraysize == 1){

        		// find whether the word exists in the index
        		if (indx_find(indx, words[0]) == NULL) {
        			// if it doesnt exist, then no documents match
        			printf("No documents match the query. \n");   
        			counters_delete(ctrset_temp);   
        				
        		}

        		else {
        			// otherwise, rank the pages. 
        			ranker(ctrset_temp, num_of_matches, pageDirectory);
	        		counters_delete(ctrset_temp);
	        		
        			}

        		}
		} 

	printf("------------------------------------------------------------------------------------------\n");
	printf("Query? ");
	free(input);
	
	}

	return 0;
}


// parses and cleans the input from user

int parse_clean(char * input, char * array[], int *actualarraysize){

	// the maximum size we allow the array to be (for testing purposes)
	int arrlength = 0;
  	char * token; 

   	token = strtok(input, " ");
   	/* walk through other tokens */
   	while(token != NULL ) {
   		if (arrlength < 700) { 
   			// store the token in the array at position 1
   			array[arrlength++] = token;
   			(* actualarraysize) ++; 
   			// increment position by 1, for the next string to be stored
   		}
   		token = strtok(NULL, " ");

   	}


   	// check if the string was empty 
   	if (arrlength == 0) {
        // printf("Error: empty query.\n");
        return 1;
    }

    // Now loop through the words, and make sure there are no non-alphabetical characters
    for (int i = 0; i < arrlength; i++) {
    	
    	for (char * x = array[i]; *x != '\0'; x++) {
    		 if (!isalpha( *x )) {
    		 	fprintf(stderr, "Error: bad character '%c' in query.\n", *x);
           		return 2;
    		 }	
    	}
    	NormalizeWord(array[i]);
    	
    }

    // Check that the array does not begin with AND or OR
    if(strcmp(array[0], "and") == 0 || strcmp(array[0], "or") == 0){
    	fprintf(stderr, "Error: '%s' cannot be first\n", array[0]);
;    	return 3; 
    }

    // Check that the array does not end with AND or OR
    if(strcmp(array[arrlength - 1], "and") == 0 || strcmp(array[arrlength - 1], "or") == 0){
    	fprintf(stderr, "Error: '%s' cannot be first\n", array[arrlength - 1]);
    	return 4; 
    }

    // Check that AND OR are not adjacent
    for (int i = 0; i < arrlength; i++) {
    	if (strcmp(array[i], "and") == 0 || strcmp(array[i], "or") == 0) {
    		if (strcmp(array[i+1], "and") == 0 || strcmp(array[i+1], "or") == 0){
    			fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", array[i], array[i+1]);
    			return 5;
    		} 
   		}
	}

	return 0;

}

// Function that performs the intersection of two countersets
static void counters_intersect(void * ctrs_pair, const int doc_ID, int count){
    int countB = counters_get((counters_t*)(((countersetpair_t*)(ctrs_pair)) -> ctrs_loop), doc_ID);

    //printf("count B: %d\n", countB);
    // taking the minimum of the count of both sets
    if (countB < count) {
        counters_set(((counters_t*)((countersetpair_t*)(ctrs_pair))->ctrs_base), doc_ID, countB);
    }
}


// function for copying the contents of one counter to another
static void countercopy(void * arg, const int doc_ID, int count){
	counters_t * intersected = arg;
	counters_set(intersected, doc_ID, count);
}

// function to merge two countersets ( i.e. add up their counts)
static void union_(void * unionizedset, const int key, int count){
    if (count != 0){
    	int countB = counters_get((counters_t*) unionizedset, key);

    	//printf("count B: %d\n", countB);
    	counters_set((counters_t*) unionizedset, key, countB + count);
    }
}


// function which counts how many keys there are in the counterset
static void counting(void *t, const int doc_ID, int count){
    if (count > 0) {
        int *total = t;
        (*total)++;
    }
}


// adapted from quicksort algorithm from geeks4geeks

static void sort_array(docID_score_t *dIs[], int lo, int hi) {
    
    if (lo < hi) {

    	/* pi is partitioning index*/

        int pi = partitioning(dIs, lo, hi);

        sort_array(dIs, lo, pi);
        sort_array(dIs, pi + 1, hi);
    }
}

// this function performs ranking of the documents within the final unionized counterset.
void ranker(counters_t * ctrset, int num_matches, char * pageDirectory) {
	// iterate and call the counting function
	counters_iterate(ctrset, &num_matches, counting);
    docID_score_t *docIDscore[num_matches];

    arrPos_t * arrPos;
    // initialize the arrPos_t struct, defined at the top
  	arrPos = malloc(sizeof(arrPos_t));
    arrPos -> arr = docIDscore;
    arrPos -> pos = 0;
    
    // iterate through and add the countersets to the array
    counters_iterate(ctrset, arrPos, ctrs_into_arr);

    int num_of_matches_sub = num_matches - 1;
    sort_array(docIDscore, 0, num_of_matches_sub);

    // if there are matches 
    if (num_matches > 0){

    	fprintf(stdout, "Matches %d documents (ranked):\n", num_matches);

    	int f = 0;
    	while (f < num_matches) {
    		char * URL;
    		// print out as per instructionis
    		printf("score %4d doc %4d: ", docIDscore[f]->rankscore, docIDscore[f]->doc_ID);
            URL = returnURL(pageDirectory, docIDscore[f]->doc_ID);
            if (URL != NULL) {
            	printf("%s\n", URL);

            }
            
            free(URL);
    		f++;
    	}
    } 

    else {
	    printf("No documents match the query.\n");
        }

    for (int ctrset = 0; ctrset < num_matches; ctrset++) {
	            	// free the docID_score_ts structs within the array 
        free(docIDscore[ctrset]);
    }

    free(arrPos);
}


// Function that retrieves an URL, given a pagedirectory and a document ID
// reads the first line of the filename: <pagedirectory>/<doc_ID> and returns the URL that is read

char * returnURL(char * pageDirec, int doc_ID){

	char * stringID = itoa(doc_ID);

	// allocate space for the filepath to be saved to
	char * filepath = calloc((strlen(pageDirec) + strlen(stringID) + 2), sizeof(char));

	// create the file path made by pagedirectory/documentID number
    sprintf(filepath, "%s/%s", pageDirec, stringID);
    

    // if the filepath turns out to be NULL, make sure to free the variable and return 
    if (filepath == NULL) {
        //free(filepath);
        fprintf(stderr, "Unable to create the file path.\n");
        return NULL;
    }
    
    // See if the file is openable
    

    FILE * fp;
    if ((fp = fopen(filepath, "r")) == NULL) {
    	fprintf(stderr, "Unable to read from the file path\n");
        free(filepath);
        return NULL;
    }
    
    // Reads first line to find the URL and returns it. 

    char * readURL = freadlinep(fp);
    fclose(fp);
    free(filepath);

    return readURL;
}


// This function puts the last item of the array as the pivot, places the pivot item in the correct position within the sorted array. It also then puts all items smaller than pivot to the left, and all greater to the right. It is a helper function to the sort_array() function
   
// Adapted from the quicksort algorithm from geeksforgeeks

static int partitioning(docID_score_t *dIs[], int lo, int hi) {
	// setting integers to represent the parameters
	int x = lo; 
    int y = hi;
	// setting the pivot
    int pivot = dIs[lo]->rankscore;
    
    
    while (true) {

        while ((dIs[x]->rankscore) > pivot){
        	x = x + 1;
        }

        while ((dIs[y]->rankscore) < pivot){
        	y = y - 1;
        } 

        if (x < y) {

            docID_score_t* placeholder = malloc(sizeof(docID_score_t));

            //swap the array[i] and array[j]
            * placeholder = * dIs[x]; 
            * dIs[x] = * dIs[y]; 
            * dIs[y] = * placeholder;

            free(placeholder);

            x = x + 1 ; 
            y = y - 1;

        } 
        else {
            return y;
        }
    }
}

// function to add the countersets into the array
// This function was written with help by my tutor, Ugur. 
static void ctrs_into_arr(void *arrPos, const int key, int c){

	arrPos_t * arrayPos = arrPos;

    if (c > 0) {

        docID_score_t * x = malloc(sizeof(docID_score_t));

        x -> doc_ID = key;
        x -> rankscore = c;
        
        int *t = &(arrayPos -> pos);

       (arrayPos->arr)[*t] = x;
       // (arrPos -> arr)[*t] = x;

        (*t)++;
    }
}



// Function to delete the counterset

static void KIdelete(void *i){

	counters_t * ct = i;
  	if (ct != NULL) {
    	counters_delete(ct);
  	}

}



/*counters_iterate(unionized, &num_of_matches, counting);
	            docID_score_t *docIDscore[num_of_matches];

	            arrPos_t * arrPos;

	          	arrPos = malloc(sizeof(arrPos_t));
	            arrPos -> arr = docIDscore;
	            arrPos -> pos = 0;
	            
	            counters_iterate(unionized, arrPos, ctrs_into_arr);

	            int num_of_matches_sub = num_of_matches - 1;
	            sort_array(docIDscore, 0, num_of_matches_sub);

         
	            if (num_of_matches > 0){

	            	fprintf(stdout, "Matches %d documents (ranked):\n", num_of_matches);

	            	int f = 0;
	            	while (f < num_of_matches) {
	            		char * URL;

	            		printf("score %4d doc %4d: ", docIDscore[f]->rankscore, docIDscore[f]->doc_ID);
	                    URL = returnURL(pageDirectory, docIDscore[f]->doc_ID);
	                    printf("%s\n", URL);
	                    

	            		f++;
	            	}*/

