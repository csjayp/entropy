#
# Makefile for entropy
#
CFLAGS+=	-O1 -Wall
OBJS=	entropy.o
LIBS=	-lm
CC?=	CC
PROG=	entropy
PREFIX?=/usr/local

entropy:	$(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)

install:
	cp entropy.1.gz $(PREFIX)/man/man1/
	cp entropy $(PREFIX)/bin

deinstall:
	rm -f $(PREFIX)/man/man1/entropy.1.gz
	rm -f $(PREFIX)/bin/entropy

clean:
	rm -f *.o $(PROG)

all:	entropy
