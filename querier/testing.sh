#!/bin/bash
# Luca Lit
# Testing.sh - a test for querier of TSE
# Test cases that should fail


echo "Case 1: Testing Incorrect usage"
./querier
echo

echo "Case 2: Invalid page directory" 
./querier invalidpagedir somerandom
echo

echo "Case 3: File not produced by crawler" 
./querier ../indexer/testingfile1 somerandom
echo

echo "Case 4: Indexfilename does not exist"
./querier http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/letters-depth-4/ http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/letters-deqdfq
echo

echo "Now for test cases that should pass (past correct usage)"

echo "Testing ./querier ~cs50/data/tse-output/letters-depth-5 ~cs50/data/tse-output/letters-index-5: "

./querier ~cs50/data/tse-output/letters-depth-5 ~cs50/data/tse-output/letters-index-5 < testing

echo "Output saved to testing.out"

valgrind ./querier ~cs50/data/tse-output/letters-depth-5 ~cs50/data/tse-output/letters-index-5 < testing1

