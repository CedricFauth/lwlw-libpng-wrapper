
CC=gcc
CFLAGS=-I.

all: tests clean

tests: tests.o 
	$(CC) -o tests tests.o -lm thirdparties/libpng/lib/libpng.a thirdparties/zlib/lib/libz.a


tests.o: tests.c
	$(CC) -c tests.c

clean:
	rm -f *.o

