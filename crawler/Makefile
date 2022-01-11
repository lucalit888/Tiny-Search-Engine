#
# Makefile for 'crawler' module
#
# Luca Chun Lun Lit

PROG = crawler
OFILES = crawler.o
LIBS = ../libcs50/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
 TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
CC = gcc
MAKE = make

$(PROG): $(OFILES)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o : crawler.c


.PHONY: test clean 

test: $(PROG)
		bash -v testing.sh &> testing.out 


clean: 
		rm -rf *.dSYM
		rm -f $(PROG)
		rm -f *~ *.o
		rm -f core 
		rm -rf data		1,1           Top
