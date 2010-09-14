PROGS=icnctrl
all: $(PROGS)

icnctrl: icnctrl.c $(KERNELDIR)/drivers/isdn/icn/icn.h
	$(CC) $(PCFLAGS) -I $(KERNELDIR)/drivers/isdn/icn -o icnctrl icnctrl.c

install: all
	install -o root -g bin -m 0750 $(PROGS) /sbin

clean:
	rm -f *.[iso] $(PROGS) *~ core

