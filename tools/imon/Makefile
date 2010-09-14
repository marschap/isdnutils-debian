#
# Makefile for iMON
#
# (C) Michael Knigge, 1995
#
#
CFLAGS    = -Wall -O2 -I$(ISDN_INCLUDE) -I/usr/include/ncurses
LFLAGS    = -lncurses
MODULES   = imon.o

imon :	$(MODULES)
	$(CC) $(CFLAGS) $(MODULES) $(LFLAGS) -o imon

install:
	install -o root -g bin -m 0750 imon /sbin

clean:
	rm -f *.o *~ imon

