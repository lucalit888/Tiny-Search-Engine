/* word.c - program that implements NormalizeWord - converting all letters in a word to lower case
 *
 * Name: Luca Lit 
 * Project: Indexer
 *
 * Date: Feb 15th 2020 
 * 
 */
#include <stdio.h>
#include <ctype.h>

// A function that normalises a word, meaning, turns it into lowercase 
char * NormalizeWord(char * string){
  char *S = string;

  while(*S) {
  	// making use of tolower() function in ctype.h
     *S = tolower(*S);
      S++;
  }
  return string;
}

int check_alphabetical(char *word){

    for (char * x = word; *x != '\0'; x++) {
        if (!isalpha( *x )) {
           fprintf(stderr, "Error: bad character '%c' in query.\n", *x);
           return 1;
        }
    }
    return 0; 
}