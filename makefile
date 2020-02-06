
CC=gcc
CFLAGS=-I.

all: mac clean

# TODO add compilation: liblwlw.a

# all in 1 compilation

mac: test.o lwlw.o src/lwlw.h
	$(CC) -o test test.o lwlw.o ext/mac/libpng/lib/libpng.a ext/mac/zlib/lib/libz.a


test.o: src/test.c src/lwlw.h
	$(CC) -c src/test.c

lwlw.o: src/lwlw.c src/lwlw.h
	$(CC) -c src/lwlw.c

clean:
	rm -f *.o

