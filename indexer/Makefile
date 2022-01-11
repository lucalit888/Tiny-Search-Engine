# Makefile for indexer 
#   Builds and compiles all these utility functions 
#
# Luca Lit

PROG = indexer indextest
OBJS = indexer.o
OBJSTEST = indextest.o
LIBS = ../common/common.a ../libcs50/libcs50.a 

FLAGS = -DMEMTEST 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common

MAKE = make

all: $(PROG)

indexer: $(LIBS) $(OBJS) 
	gcc $(CFLAGS) $(OBJS) $(LIBS) -o indexer 

indextest: $(LIBS) $(OBJSTEST) 
	gcc $(CFLAGS) $(OBJSTEST) $(LIBS) -o indextest

indexer.o: indexer.c ../common/pagedir.h ../common/index.h
indextest.o: indextest.c ../common/pagedir.h ../common/index.h

.PHONY: test valgrind clean
	all: indexer indextest

test: $(PROG) 
	bash ./testing.sh &> testing.out


clean:
	rm -rf *.dSYM  
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f somefilename
	
	rm -f core
	rm -f testingfile1 
	rm -f testingfile2
	rm -f testingfile1.sorted
	rm -f testingfile2.sorted
