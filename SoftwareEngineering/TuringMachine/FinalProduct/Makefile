CC=gcc

CFLAGS=-Wstrict-prototypes -Wall -g
LDFLAGS=-g -lrt

TuringMachineSimulator: TuringMachineSimulator.o
	$(CC) -g -o TuringMachineSimulator TuringMachineSimulator.o

TuringMachineSimulator.o: TuringMachineSimulator.c
	$(CC) -c $(CFLAGS) $< 
	
.PHONY: clean
clean:
	rm -f ./*.o

.PHONY: immaculate
immaculate: clean
	rm -f ./TuringMachineSimulator
