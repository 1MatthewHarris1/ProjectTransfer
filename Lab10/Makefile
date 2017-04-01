CC=gcc

CFLAGS=-Wstrict-prototypes -Wall -g
LDFLAGS=-g -lrt

all: experiment_tmm_pt1 experiment_tmm_pt2.o experiment_tmm_pt2 mat_mul_pt2.o mat_mul_pt2

experiment_tmm_pt1: experiment_tmm_pt1.o mat_mul_pt1.o tspec_diff.o
	$(CC) -g -o experiment_tmm_pt1 experiment_tmm_pt1.o mat_mul_pt1.o -lm

experiment_tmm_pt2.o: experiment_tmm_pt2.c mat_mul_pt2.c tspec_diff.c
	$(CC) -c $(CFLAGS) $< 

experiment_tmm_pt2: experiment_tmm_pt2.o mat_mul_pt2.o  tspec_diff.o
	$(CC) -g -o experiment_tmm_pt2 experiment_tmm_pt2.o mat_mul_pt2.o -lm

mat_mul_pt2.o: mat_mul_pt2.c
	$(CC) -c $(CFLAGS) $< 
	
mat_mul_pt2: mat_mul_pt2.o
	$(CC) -g -o mat_mul_pt2 mat_mul_pt2.o -lm

.PHONY: clean
clean:
	rm -f ./*.o

.PHONY: immaculate
immaculate: clean
	rm -f ./experiment_tmm_pt1
	rm -f ./experiment_tmm_pt2
	rm -f ./mat_mul
