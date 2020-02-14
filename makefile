
CC=gcc
CFLAGS=-std=c99 -Wall
LIB1=$(wildcard ext/mac/libpng/obj/*.o)
LIB2=$(wildcard ext/mac/zlib/obj/*.o )

# TODO add compilation: liblwlw.a

# all in 1 compilation

linux: clean1 comp_linux clean2

mac: clean1 comp_mac clean2

mac2: clean1 comp_mac_2 clean2

comp_mac: test.o lwlw.o src/lwlw.h
	$(CC) $(CFLAGS) -o test test.o lwlw.o -Lext/mac/libpng/lib/ -Lext/mac/zlib/lib/ -lpng -lz -lm

comp_mac_2: test.o src/lwlw.h maclib
	$(CC) $(CFLAGS) -o test2 test.o -Lbin/ -llwlw -lm

comp_linux: test.o lwlw.o src/lwlw.h
	$(CC) $(CFLAGS) -o test test.o lwlw.o ext/linux/libpng/lib/libpng.a ext/linux/zlib/lib/libz.a -lm

libmac: lwlw.o src/lwlw.h
	ar rcs bin/liblwlw.a lwlw.o $(LIB1) $(LIB2)

test.o: src/test.c src/lwlw.h
	$(CC) $(CFLAGS) -c src/test.c -I ext/mac/libpng/include

lwlw.o: src/lwlw.c src/lwlw.h
	$(CC) $(CFLAGS) -c src/lwlw.c -I ext/mac/libpng/include


.PHONY: clean1 clean2
clean1:
	rm -f *.o	
	
clean2:
	rm -f *.o
	
