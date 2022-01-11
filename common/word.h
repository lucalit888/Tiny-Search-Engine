/* 
 * word.h - header file 
 * 
 * Containing utility functions for converting words to lower case - common to indexer and querier
 *
 * Luca Lit - Feb 15 2020
 *
 */

#ifndef __WORD_H
#define __WORD_H

/**************** Normalize ****************/
/*
 * A function that normalises a word, i.e. converting it into lowercase. 
 *
 * Takes in a string as an input parameter, and returns a string (that has been converted to lowercase letters only.
 *
 * Makes use of the tolower() function in ctype.h
 */

char * NormalizeWord(char * string);

int check_alphabetical(char *array);


#endif
