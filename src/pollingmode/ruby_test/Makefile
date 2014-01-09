CC = /usr/bin/gcc
SHFLAGS = -Wall -pedantic -O3 -c -fPIC
CFLAGS = -Wall -g -pedantic -O3 -o nfc_beta
LIBS = -pthread

all: nfclib.c nfc_beta.c
	$(CC) $(CFLAGS) $(LIBS) nfclib.c nfc_beta.c
	$(CC) $(SHFLAGS) $(LIBS) nfclib.c
	$(CC) -shared -o nfclib.so nfclib.o
install: nfclib.c nfc_beta.c
	$(CC) $(CFLAGS) $(LIBS) nfclib.c nfc_beta.c
	$(CC) $(SHFLAGS) $(LIBS) nfclib.c
	$(CC) -shared -o nfclib.so nfclib.o
	cp nfc_beta ~/bin
	mv nfc_beta ../../bin
clean:
	rm nfc_beta
