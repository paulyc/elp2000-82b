CC=gcc
LD=gcc
CFLAGS=-std=c11 -D_USE_MATH_DEFINES=1
LDFLAGS=-lm
DEPS = arguments.h earthfig.h elp2000-82b.h mainprob.h moonfig.h planetary1.h planetary2.h precession.h relativistic.h series.h sidereal_time.h solarecc.h tidal.h

all: moonphase

elp2000.a: arguments.o elp2000-82b.o precession.o series.o sidereal_time.o
	ar rcs elp2000.a arguments.o elp2000-82b.o precession.o series.o sidereal_time.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

moonphase: elp2000.a moonphase.o
	$(LD) -g -o moonphase moonphase.o elp2000.a $(LDFLAGS)

clean:
	rm -f *.o moonphase elp2000.a
.PHONY: clean
