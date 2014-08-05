CC = /usr/bin/gcc
SHFLAGS = -Wall -pedantic -O3 -c -fPIC
LIBS = -pthread

all: nfclib.c
	$(CC) $(SHFLAGS) $(LIBS) nfclib.c 
	$(CC) -shared -o nfclib.so nfclib.o
	mv nfclib.so ../lib/nfclib.so
install: nfclib.c 
	$(CC) $(SHFLAGS) $(LIBS) nfclib.c 
	$(CC) -shared -o nfclib.so nfclib.o
	mv nfclib.so ../lib/nfclib.so
