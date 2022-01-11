# CS50 Lab 4 - TESTING.md for TSE crawler 
## Luca Lit, CS50 Winter 2020

### This markdown file details the testing that is executed through testing.sh

Test cases include: 

**SHOULD FAIL**

1. Testing with one argument 
2. Testing with two arguments 
3. Testing with 3 arguments 
4. Testing with 4 arguments and a non existent server
5. Testing with 4 arguments and an external URL
6. Testing with 4 argument and valid server but nonexistent page 
7. Testing with  4 arguments and an invalid directory to save into


**SHOULD PASS**

8. Testing 4 arguments with seedURL and depth 0
9. Testing 4 arguments with seedURL and depth 1
10. Testing 4 arguments with seedURL and depth 2
11. Testing 4 arguments with seedURL and depth 3
12. Testing 4 arguments with seedURL and depth 4
13. Testing 4 arguments with seedURL and depth 5
14. Testing different seedURL in same site
15. Testing the wikipedia playground at depth 0 
16. Valgrinding an example to show that there are no memory leaks.





