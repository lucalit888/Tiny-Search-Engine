# Makefile for querier 
#
# Name: Luca Lit
# Date: Feb 20 2020

PROG = querier 
OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50.a 

FLAGS = -DMEMTEST 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common

MAKE = make

all: $(PROG)

querier: $(LIBS) $(OBJS) 
	gcc $(CFLAGS) $(OBJS) $(LIBS) -o querier 

querier.o: querier.c ../common/pagedir.h ../common/index.h 

.PHONY: test valgrind clean
	all: querier

test: $(PROG) 
	bash ./testing.sh &> testing.out


clean:
	rm -rf *.dSYM  
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core

