VERSION=0.3

# the directory where isdn.h can be found
#ISDN_INCLUDE=$(INCLUDE)


SBINPATH=/sbin
MAN8PATH=$(MANDEST)/man8

CCFLAGS=-I$(ISDN_INCLUDE) -I/usr/include/g++-include -O6
LDFLAGS=

# nothing to change behind this line

PROGS=imontty
MAN8FILES=imontty.8

all: $(PROGS)

imontty: imontty.o 
	g++ $(CCFLAGS) -o imontty imontty.o $(LDFLAGS)

imontty.o: imontty.cc 
	g++ $(CCFLAGS) -c imontty.cc

install: $(PROGS)
	@strip $(PROGS)
	install -o root -g root -m 0755 $(PROGS) $(SBINPATH)
	install -o root -g root -m 0644 $(MAN8FILES) $(MAN8PATH)

clean:
	-@rm -f *.o $(PROGS) *~


