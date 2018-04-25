C=gcc
CFLAGS=-I.

permute: gen_perms.o permute.o
	$(CC) -o permute gen_perms.o permute.o -I.

.PHONY: clean

clean:
	rm ./gen_perms.o
	rm ./permute.o
