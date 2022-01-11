#!/bin/bash

# Testing script for crawler.c
# Author: Luca Lit
# Date: Feb 7, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
diffseedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
externalURL=www.google.com
Wiki=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/

# Uncomment the following if you want to test them

#seedURL2=http://old-www.cs.dartmouth.edu/~cs50/index.html
#seedURL3=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
#seedURL4=http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html

# making a directory called data to store testfiles in 
mkdir data
mkdir ./data/testA0
mkdir ./data/testA1
mkdir ./data/testA2
mkdir ./data/testA3
mkdir ./data/testA4
mkdir ./data/testA5
mkdir ./data/testB
mkdir ./data/testC

echo

#####################################
### Test cases that should fail ###

# 1 argument
echo "Testing with 1 argument:"
./crawler
echo

# 2 arguments
echo "Testing with 2 arguments:"
./crawler $seedURL
echo

# 3 arguments
echo "Testing with 3 arguments:"
./crawler $seedURL ./data/testA0
echo

# 4 arguments + non existent server
echo "Testing with 4 arguments + non-existent server:"
./crawler invalidserver ./data/testA0 2
echo

# 4 arguments + externalURL
echo "Testing with 4 arguments + externalURL:"
./crawler $externalURL ./data/testA0 2
echo

# 4 arguments + valid server but non-existent page
echo "Testing with 4 arguments + valid server but non-existent page:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/ ./data/testA0 2
echo

# 4 arguments + invalid page directory
echo "Testing with 4 arguments + invalid directory to save into:"
./crawler $seedURL randomfilename 2
echo


######################################
### Test cases that should pass ####

# seedURL at depth 0
echo "Testing 4 arguments + seedURL + depth(0): "
./crawler $seedURL ./data/testA0 0
echo "Results saved to ./data/testA0"
echo

# seedURL at depth 1
echo "Testing 4 arguments + seedURL + depth(1): "
./crawler $seedURL ./data/testA1 1
echo "Results saved to ./data/testA1"
echo

# seedURL at depth 2
echo "Testing 4 arguments + seedURL + depth(2): "
./crawler $seedURL ./data/testA2 2
echo "Results saved to ./data/testA2"
echo

# seedURL at depth 3
echo "Testing 4 arguments + seedURL + depth(3): "
./crawler $seedURL ./data/testA3 3
echo "Results saved to ./data/testA3"
echo

# seedURL at depth 4
echo "Testing 4 arguments + seedURL + depth(4): "
./crawler $seedURL ./data/testA4 4
echo "Results saved to ./data/testA4"
echo

# seedURL at depth 5
echo "Testing 4 arguments + seedURL + depth(5): "
./crawler $seedURL ./data/testA5 5
echo "Results saved to ./data/testA5"
echo

# different seedURL in same site
echo "Testing 4 arguments + different seedpage in same site: "
./crawler $diffseedURL ./data/testB 1
echo "Results saved to ./data/testB"
echo

# Wikipedia playground at depth 0
echo "Testing 4 arguments + Wikipedia playground + depth(0): "
./crawler $Wiki ./data/testC 0
echo "Results saved to ./data/testC"
echo

# valgrinding 
echo "Example of calgrinding to make sure there are no memory leaks: "
valgrind ./crawler $seedURL ./data/testA2 2

