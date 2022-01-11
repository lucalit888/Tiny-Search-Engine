/* index.h - header file for index.c 
*
* Utility functions for saving and loading index files (this module is common between the Indexer, Querier, and indextest);
*
* Name: Luca Lit
* Date: 16 Feb 2020
*
*/

#ifndef __INDEX_H
#define __INDEX_H

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
#include "word.h"
#include "pagedir.h"

/**************** functions ****************/

/**************** indx_t ****************/
// Defining index_t as the same as hashtable_t
typedef hashtable_t indx_t;

/**************** indx_new ****************/
/*
* indx_new takes one parameter, which is a constant integer that represents the number of slots
* This function simply calls hashtable_new() and returns it
*/

indx_t *indx_new(const int num_of_slots);

/**************** indx_insert ****************/
/* 
 * indx_insert takes in the parameters of a hashtable, a string, and a counterset struct
 * Function that inserts a counterset, given an index and a key
 * since hashtable insert returns bool, our func type is also bool
 *
 * It essentially calls hashtable_insert() and inputs the above parameters
 */

bool indx_insert(indx_t *indx, char *key, counters_t *counts);


/**************** indx_find ****************/
/*
 * Return the counterset associated with the given key.
 *
 * The parameters are: a pointer to hashtable, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the hashtable is unchanged by this operation.
 * Essentially replaces the void* item in hashtable_find to counters_t* item
*/

counters_t* indx_find(indx_t* indx, char* key);

/**************** index_saver ****************/
/*
* Takes in a hashtable and a string filename as parameter
* Is of type void, as such, returns nothing 
*
* This function is for saving to the index file - 
* Opens the file to be written into, and makes use of hashtable_iterate() to call item_saver on every single element in the index hashtable
* 
*/

void index_saver(char* nameoffile, indx_t* indx);

/**************** key_saver ****************/
/*
* Takes in as parameters, a void arg, which corresponds to the file that we are writing into, then a string key and a void pointer to an item
* Prints the item (which is the word), then calls count_saver, which wil print key and counts
*/

void key_saver(void *arg, const char *key, void *i);

/**************** count_saver ****************/
/*
*This function takes in the same parameters as the key_saver, except it prints the key: count pairs for every item. 
*The key count pair represents the docID and number of times the  word appears
*This function does not return anything, it is of type void
*/
void count_saver(void *arg, const int key, int count);


/**************** index_loader ****************/
/*
* Input Parameter is a filename and an empty index file - and returns a hashtable, (or index struct)
* 
* It is of type indx_t*, which is defined earlier as a hashtable
*
* The function takes and opens an indexfile, a file produced by the indexer, and reads it.
* Afterwards it creates an inverted index structure. The output will be used for index_saver. 
*/
indx_t* index_loader(char * nameoffile, indx_t * indx);

#endif