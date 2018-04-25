CC=gcc

except_demo: except.o except_demo.o
	$(CC) -Wall -Wstrict-prototypes -o except_demo except.o except_demo.o

find_file: find_file.o except.o
	$(CC) -Wall -Wstrict-prototypes -o find_file find_file.o except.o

.PHONY: clean

clean:
	rm ./except.o
	rm ./except_demo.o
	rm ./find_file.o

.PHONY: immaculate

immaculate:
	rm ./except.o
	rm ./except_demo.o
	rm ./except_demo
	rm ./find_file.o
	rm ./find_file
