CC=gcc

CFLAGS=-Wstrict-prototypes -Wall -g
LDFLAGS=-g

tattle.o:	tattle.c
	$(CC) -c $(CFLAGS) $<

tattle:	tattle.o
	$(CC) -g -o tattle tattle.o

.PHONY: clean
clean:
	rm -f ./tattle.o

.PHONY: immaculate
immaculate: clean
	rm -f ./tattle
