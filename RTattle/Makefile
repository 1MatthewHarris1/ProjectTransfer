CC=gcc

CFLAGS=-Wstrict-prototypes -Wall -g
LDFLAGS=-g

tattle:	tattle.o
	$(CC) -g -o tattle tattle.o

tattle.o:	tattle.c
	$(CC) -c $(CFLAGS) $<

get_sockaddr.o: get_sockaddr.c
	$(CC) -c $(CFLAGS) $<

get_sockaddr:	get_sockaddr.o
	$(CC) -g -o get_sockaddr get_sockaddr.o

remsh.o: remsh.c
	$(CC) -c $(CFLAGS) $< 

remsh:	remsh.o get_sockaddr.o
	$(CC) -g -o remsh remsh.o get_sockaddr.o

tattled.o:	tattled.c get_sockaddr.c
	$(CC) -c $(CFLAGS) $< 

tattled:	tattled.o daemonize_me.o
	$(CC) -g -o tattled tattled.o daemonize_me.o

daemonize_me.o:	daemonize_me.c
	$(CC) -c $(CFLAGS) $<
	

.PHONY: clean
clean:
	rm -f ./sockaddr.o
	rm -f ./remsh.o
	rm -f ./remshd.o
	rm -f ./daemonize_me.o
	rm -f ./tattle.o

.PHONY: immaculate
immaculate: clean
	rm -f ./get_sockaddr
	rm -f ./remsh
	rm -f ./remshd
	rm -f ./tattle
