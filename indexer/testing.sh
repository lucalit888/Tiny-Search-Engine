#!/bin/bash

# CS50 Lab 5 - Indexer - testing.sh
# Luca Lit - Feb 16 2020
#
# Usage: ./testing.sh


# Cases that should fail
echo
echo "********************** TESTING FILE OUTPUT *************************"
echo 
echo "--------------------------------------------------------------------"
echo "1) Test result for invalid number of arguments supplied by user:"
echo
echo "Testing ./indexer 6 6 3"
./indexer 6 6 3
echo "Testing ./indextest 5 3 5 "
./indextest 5 3 5

echo
echo "--------------------------------------------------------------------"
echo "2) Testing incorrect inputs (i.e. invalid directory):"
echo 
echo "Testing ./indexer falsedirectory somefilename"
./indexer falsedirectory somefilename

echo
echo "--------------------------------------------------------------------"
echo "3) Testing incorrect inputs (i.e. directory not produced by crawler):"
echo 
echo "Testing ./indexer ../libcs50 somefilename"
./indexer ../libcs50 somefilename

echo

# Cases that should pass
echo "--------------------------------------------------------------------"
echo "4) Testing a correct example on the following directory '~cs50/data/tse-output/letters-depth-1' : "
echo 
echo "4(a) Testing ./indexer ~cs50/data/tse-output/letters-depth-1 testingfile1:"
echo
./indexer ~cs50/data/tse-output/letters-depth-1 testingfile1
#./indexer ~cs50/data/tse-output/wikipedia-depth-0 testingfile1
echo "Verifying that indexer worked:"
echo 
cat testingfile1
echo
echo "--------------------------------------------------------------------"
echo "4(b) Testing ./indextest testingfile1 testingfile2:"
./indextest testingfile1 testingfile2
echo
echo "To verify  indextest works well, we will look at the results of both files:"
echo
echo "Results of testingfile1 :"
echo
cat testingfile1
echo
echo "Results of testingfile2 :"
echo
cat testingfile2
echo 
echo "--------------------------------------------------------------------"
echo "4(c) Testing if there are any differences between the two files: "
echo
echo "Sorting the two files and comparing them ..."
gawk -f indexsort.awk testingfile2 > testingfile1.sorted
gawk -f indexsort.awk testingfile2 > testingfile2.sorted

diff -q testingfile1.sorted testingfile2.sorted

	if [[ $? == 0 ]]; then
		echo "There are no differences."
		echo
	fi
echo "--------------------------------------------------------------------"
echo "Valgrinding to show there are no memory leaks: "
echo
valgrind ./indexer ~cs50/data/tse-output/letters-depth-4 testingfile1
