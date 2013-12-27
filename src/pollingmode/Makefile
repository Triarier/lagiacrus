CC = /usr/bin/gcc
CFLAGS = -Wall -g -pedantic -O3 -o rathalos
SHFLAGS = -Wall -pedantic -O3 -c -fPIC
LIBS = -pthread

all: nfclib.c rathalos.c
	$(CC) $(CFLAGS) $(LIBS) nfclib.c rathalos.c
install: nfclib.c rathalos.c
	$(CC) $(CFLAGS) $(LIBS) nfclib.c rathalos.c
	cp rathalos ~/bin
	mv rathalos ../../bin
clean:
	rm rathalos
