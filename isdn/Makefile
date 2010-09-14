PROGS=isdnctrl iprofd

all: $(PROGS)

isdnctrl: isdnctrl.c
	$(CC) $(PCFLAGS) -o isdnctrl isdnctrl.c

iprofd: iprofd.c
	$(CC) $(PCFLAGS) -o iprofd iprofd.c

install: all
	install -o root -g bin -m 0750 $(PROGS) /sbin

clean:
	rm -f *.[iso] $(PROGS) *~ core
