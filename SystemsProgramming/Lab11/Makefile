CC=gcc

CFLAGS=-Wstrict-prototypes -Wall -g
LDFLAGS=-g

all: lander_pt5 lander_pt4 lander_pt3 lander_pt2 lander_pt1

lander_pt5.o: lander_pt5.c
	$(CC) -c $(CFLAGS) $< 

lander_pt5:	lander_pt5.o
	$(CC) -g -o lander_pt5 lander_pt5.o -lm

lander_pt4.o: lander_pt4.c
	$(CC) -c $(CFLAGS) $< 

lander_pt4:	lander_pt4.o
	$(CC) -g -o lander_pt4 lander_pt4.o -lm

lander_pt3.o: lander_pt3.c
	$(CC) -c $(CFLAGS) $< 

lander_pt3:	lander_pt3.o
	$(CC) -g -o lander_pt3 lander_pt3.o -lm

lander_pt2.o: lander_pt2.c
	$(CC) -c $(CFLAGS) $< 

lander_pt2:	lander_pt2.o
	$(CC) -g -o lander_pt2 lander_pt2.o -lm

lander_pt1.o: lander_pt1.c
	$(CC) -c $(CFLAGS) $< 

lander_pt1:	lander_pt1.o
	$(CC) -g -o lander_pt1 lander_pt1.o -lm

.PHONY: clean
clean:
	rm -f ./lander_pt1.o
	rm -f ./lander_pt2.o
	rm -f ./lander_pt3.o
	rm -f ./lander_pt4.o
	rm -f ./lander_pt5.o

.PHONY: immaculate
immaculate: clean
	rm ./lander_pt1
	rm ./lander_pt2
	rm ./lander_pt3
	rm ./lander_pt4
	rm ./lander_pt5
