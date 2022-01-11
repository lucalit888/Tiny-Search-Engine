# CS50 Lab 6- Querier TESTING.md
## Luca Lit

# Test cases:

The following test cases are executed in testing.sh:

CASES THAT FAIL: 

"Case 1: Testing Incorrect usage"
./querier


"Case 2: Invalid page directory" 
./querier invalidpagedir somerandom


"Case 3: File not produced by crawler" 
./querier ../indexer/testingfile1 somerandom


"Case 4: Indexfilename does not exist"
./querier http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/letters-depth-4/ http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/letters-deqdfq

CASES THAT PASS:

Test: ./querier ~cs50/data/tse-output/letters-depth-5 ~cs50/data/tse-output/letters-index-5 < testing

Look into the testing file to find out about what was the input

Outputs are saved to testing.out