PROGRAM = HWCollatz

ASFLAGS = --gstabs+

LDFLAGS = -g

#CFLAGS = -g -fomit-frame-pointer
CFLAGS = -g -fomit-frame-pointer

$(PROGRAM): HWCollatz.o
	$(CC) $(LDFLAGS) -o $@ $^

gcd.o: HWCollatz.s
	$(AS) $(ASFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o *~ a.out core

immaculate: clean
	rm -f $(PROGRAM)
