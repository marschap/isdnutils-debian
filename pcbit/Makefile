HDRS=pcbit.h edss1.h callbacks.h layer2.h capi.h
EXTRA=pcbitctl.1 README

CTLSRCS=pcbitctl.c convhexbin.c
CTLOBJS=pcbitctl.o convhexbin.o

NFLAGS=-Wall -Wstrict-prototypes -g -m486 -I$(KERNELDIR)/drivers/isdn/pcbit -DDEBUG

all: pcbitctl

pcbitctl:$(CTLOBJS)
	gcc -g -o pcbitctl $(CTLOBJS)

pcbitctl.o:pcbitctl.c $(KERNELDIR)/drivers/isdn/pcbit/pcbit.h
	gcc $(NFLAGS) -c pcbitctl.c

convhexbin.o:convhexbin.c
	gcc $(NFLAGS) -c convhexbin.c

clean:
	rm -f $(OBJS) *.[iso] *~ convhexbin pcbitctl

tar:
	tar -cvf pcbit.tar $(SRCS) $(HDRS) $(CTLSRCS) $(EXTRA) Makefile

install: all
	install -o root -g bin -m 0750 pcbitctl /sbin
