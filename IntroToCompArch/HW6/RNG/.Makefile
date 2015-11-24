PROGRAM = rng

ASFLAGS = --gstabs+

LDFLAGS = -g

#CFLAGS = -g -fomit-frame-pointer
CFLAGS = -g -fomit-frame-pointer

$(PROGRAM): rng.o
	$(CC) $(LDFLAGS) -o $@ $^

gcd.o: rng.s
	$(AS) $(ASFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o *~ a.out core

immaculate: clean
	rm -f $(PROGRAM)
