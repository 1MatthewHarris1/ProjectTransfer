CC=gcc

killme_pt1: killme_pt1.o
	$(CC) -Wall -Wstrict-prototypes -o killme_pt1 killme_pt1.o

killme_pt2: killme_pt2.o
	$(CC) -Wall -Wstrict-prototypes -o killme_pt2 killme_pt2.o

killme_pt3: killme_pt3.o critical_section.o
	$(CC) -Wall -Wstrict-prototypes -o killme_pt3 killme_pt3.o critical_section.o

.PHONY: clean

clean:
	rm ./killme_pt1.o
	rm ./killme_pt2.o
	rm ./killme_pt3.o

.PHONY: immaculate

immaculate:
	rm ./killme_pt1.o
	rm ./killme_pt2.o
	rm ./killme_pt3.o
	rm ./killme_pt1
	rm ./killme_pt2
	rm ./killme_pt3
