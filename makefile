
CC=gcc
CFLAGS=-std=c99 -Wall

# TODO add compilation: liblwlw.a

# all in 1 compilation

linux: clean1 comp_linux clean2

mac: clean1 comp_mac clean2

comp_mac: test.o lwlw.o src/lwlw.h
	$(CC) $(CFLAGS) -o test test.o lwlw.o ext/mac/libpng/lib/libpng.a ext/mac/zlib/lib/libz.a -lm

comp_linux: test.o lwlw.o src/lwlw.h
	$(CC) $(CFLAGS) -o test test.o lwlw.o ext/linux/libpng/lib/libpng.a ext/linux/zlib/lib/libz.a -lm

test.o: src/test.c src/lwlw.h
	$(CC) $(CFLAGS) -c src/test.c -I ext/mac/libpng/include

lwlw.o: src/lwlw.c src/lwlw.h
	$(CC) $(CFLAGS) -c src/lwlw.c -I ext/mac/libpng/include

.PHONY: clean1 clean2
clean1:
	rm -f *.o	
	
clean2:
	rm -f *.o
	
