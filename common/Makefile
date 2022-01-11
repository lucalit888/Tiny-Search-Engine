#
# Makefile for 'common' module
#
# Luca Chun Lun Lit

OBJS = word.o pagedir.o index.o
LIBS = common.a
LIBS2 = ../libcs50/libcs50.a
# uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(LIBS2) $(OBJS)

pagedir.o: pagedir.h 
word.o: word.h
index.o : index.h

.PHONY: clean

clean: 
		rm -rf *.dSYM
		rm -f $(LIBS)
		rm -f *~ *.o
		rm -f core                                                          1,1           Top
