CC=gcc
LD=ld
CFLAGS=-I.
LDFLAGS=-lm
DEPS = arguments.h earthfig.h elp2000-82b.h mainprob.h moonfig.h planetary1.h planetary2.h relativistic.h series.h solarecc.h tidal.h

elp2000.a: arguments.o elp2000-82b.o series.o
	ar rcs elp2000.a arguments.o elp2000-82b.o series.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

moonphase: elp2000.a moonphase.o
	$(LD) -o moonphase moonphase.o elp2000.a $(LDFLAGS)

clean:
	rm -f *.o moonphase elp2000.a
.PHONY: clean
