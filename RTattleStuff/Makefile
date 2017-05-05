CC=gcc

CFLAGS=-Wstrict-prototypes -Wall -g
LDFLAGS=-g

get_sockaddr.o: get_sockaddr.c
	$(CC) -c $(CFLAGS) $<

get_sockaddr:	get_sockaddr.o
	$(CC) -g -o get_sockaddr get_sockaddr.o

remsh.o: remsh.c
	$(CC) -c $(CFLAGS) $< 

remsh:	remsh.o get_sockaddr.o
	$(CC) -g -o remsh remsh.o get_sockaddr.o

remshd.o:	remshd.c get_sockaddr.c
	$(CC) -c $(CFLAGS) $< 

remshd:	remshd.o daemonize_me.o
	$(CC) -g -o remshd remshd.o daemonize_me.o

daemonize_me.o:	daemonize_me.c
	$(CC) -c $(CFLAGS) $<
	

.PHONY: clean
clean:
	rm -f ./sockaddr.o
	rm -f ./remsh.o
	rm -f ./remshd.o
	rm -f ./daemonize_me.o

.PHONY: immaculate
immaculate: clean
	rm -f ./get_sockaddr
	rm -f ./remsh
	rm -f ./remshd
