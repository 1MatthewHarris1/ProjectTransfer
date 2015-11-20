PROGRAMS = \
	guess \
	rng_t

# Even thought the CI20 is little-endian, we force the listing to be
# big-endian for consistency with the textbook.
LISTING_ENDIAN_FLAG = -EB

ASFLAGS = --gstabs+

LDFLAGS = -g

.PHONY: default
default: $(PROGRAMS)

rng.o: rng.s
	$(AS) $(ASFLAGS) -o $@ $^

rng_t: rng_t.o rng.o
	$(CC) $(LDFLAGS) -o $@ $^

rng_t.o: rng_t.c
	$(CC) -c -g -o $@ $^

%.list: %.s
	$(AS) -alh=$@ $(LISTING_ENDIAN_FLAG) $^

guess: guess.o rng.o
	$(CC) $(LDFLAGS) -o $@ $^

guess.o: guess.s
	$(AS) $(ASFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o *.list *~ a.out core

.PHONY: immaculate
immaculate: clean
	rm -f $(PROGRAM)