# How to write makefile
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
CFLAGS=$(shell root-config --cflags)
LIBS=$(shell root-config --glibs)
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) -I. $(LIBS)

tests: tests.o ComplexMatrix.o
	$(CC) -o $@ $^ $(CFLAGS) -I. $(LIBS)